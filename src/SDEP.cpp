#include "SDEP.h"

#include <string.h>

namespace FactUtilEmbedded
{

// Recreate Adafruit TimeoutTimer
class TimeoutTimer
{
    Timer timer;
    uint32_t timeout;

public:
    TimeoutTimer(uint32_t timeout)
    {
        this->timeout = timeout;
        timer.start();
    }

    bool expired()
    {
        return (uint32_t) timer.read_ms() < timeout;
    }

    void set(uint32_t timeout)
    {
        timer.reset();
        this->timeout = timeout;
    }

    void reset() { timer.reset(); }
};

// placeholders
#define SPI_CS_ENABLE()
#define SPI_CS_DISABLE()

#define SPI_IGNORED_BYTE          0xFEu /**< SPI default character. Character clocked out in case of an ignored transaction. */
#define SPI_OVERREAD_BYTE         0xFFu /**< SPI over-read character. Character clocked out after an over-read of the transmit buffer. */
#define SPI_DEFAULT_DELAY_US 50


template <class TSPI>
inline bool SDEP<TSPI>::irq_pin()
{
    return irq == 1;
}


/******************************************************************************/
/*!
    @brief Flush current response data in the internal FIFO

    @return -1 if no data is available
*/
/******************************************************************************/
template <class TSPI>
inline void SDEP<TSPI>::flush()
{
    // TODO
}


template <class TSPI>
void SDEP<TSPI>::spixfer(void* _buf, uint16_t count)
{
    auto buf = static_cast<char*>(_buf);
    // TODO: refactor this out and call spi class directly
    while(count--)
    {
        *buf = spi.transfer(*buf);
        buf++;
    }
}

template <class TSPI>
bool SDEP<TSPI>::sendPacket(uint16_t command, const uint8_t* buf, uint8_t count, uint8_t more_data)
{
    // MB: pretty sure Adafruit set this to 50
    constexpr uint32_t  _timeout = 50;

    // flush old response before sending the new command
    if (more_data == 0) flush();

    sdepMsgCommand_t msgCmd;

    msgCmd.header.msg_type      = SDEP_MSGTYPE_COMMAND;
    msgCmd.header.cmd_id_high = highByte(command);
    msgCmd.header.cmd_id_low  = lowByte(command);
    msgCmd.header.length      = count;
    msgCmd.header.more_data   = (count == SDEP_MAX_PACKETSIZE) ? more_data : 0;

    // Copy payload
    if ( buf != NULL && count > 0) memcpy(msgCmd.payload, buf, count);

    // Starting SPI transaction
    spi.begin();

    SPI_CS_ENABLE();

    TimeoutTimer tt(_timeout);

    // Bluefruit may not be ready
    while ( ( spi.transfer(msgCmd.header.msg_type) == SPI_IGNORED_BYTE ) && !tt.expired() )
    {
      // Disable & Re-enable CS with a bit of delay for Bluefruit to ready itself
      SPI_CS_DISABLE();
      wait_ms(SPI_DEFAULT_DELAY_US);
      SPI_CS_ENABLE();
    }

    bool result = !tt.expired();

    if ( result )
    {
      // transfer the rest of the data
      spixfer((void*) (((uint8_t*)&msgCmd) +1), sizeof(sdepMsgHeader_t)+count-1);
    }

    SPI_CS_DISABLE();

    spi.end();

    return result;
}

template <class TSPI>
bool SDEP<TSPI>::getPacket(sdepMsgResponse_t *p_response)
{
    // MB: pretty sure Adafruit set this to 50
    constexpr uint32_t  _timeout = 50;

    // Wait until IRQ is asserted, double timeout since some commands take long time to start responding
    TimeoutTimer tt(2*_timeout);

    while ( !irq_pin() ) {
        if (tt.expired()) return false;
    }

    sdepMsgHeader_t* p_header = &p_response->header;

    spi.begin();

    SPI_CS_ENABLE();

    tt.set(_timeout);

    do {
        if ( tt.expired() ) break;

        p_header->msg_type = spi.transfer(0xff);

        if (p_header->msg_type == SPI_IGNORED_BYTE)
        {
            // Bluefruit may not be ready
            // Disable & Re-enable CS with a bit of delay for Bluefruit to ready itself
            SPI_CS_DISABLE();
            wait_ms(SPI_DEFAULT_DELAY_US);
            SPI_CS_ENABLE();
        }
        else if (p_header->msg_type == SPI_OVERREAD_BYTE)
        {
            // IRQ may not be pulled down by Bluefruit when returning all data in previous transfer.
            // This could happen when Arduino MCU is running at fast rate comparing to Bluefruit's MCU,
            // causing an SPI_OVERREAD_BYTE to be returned at stage.
            //
            // Walkaround: Disable & Re-enable CS with a bit of delay and keep waiting
            // TODO IRQ is supposed to be OFF then ON, it is better to use GPIO trigger interrupt.

            SPI_CS_DISABLE();
            // wait for the clock to be enabled..
//      while (!digitalRead(m_irq_pin)) {
//        if ( tt.expired() ) break;
//      }
//      if (!digitalRead(m_irq_pin)) break;
            wait_ms(SPI_DEFAULT_DELAY_US);
            SPI_CS_ENABLE();
        }
    }  while (p_header->msg_type == SPI_IGNORED_BYTE || p_header->msg_type == SPI_OVERREAD_BYTE);

    bool result=false;

    // Not a loop, just a way to avoid goto with error handling
    do
    {
        // Look for the header
        // note that we should always get the right header at this point, and not doing so will really mess up things.
        while ( p_header->msg_type != SDEP_MSGTYPE_RESPONSE && p_header->msg_type != SDEP_MSGTYPE_ERROR && !tt.expired() )
        {
            p_header->msg_type = spi.transfer(0xff);
        }

        if ( tt.expired() ) break;

        memset( (&p_header->msg_type)+1, 0xff, sizeof(sdepMsgHeader_t) - 1);
        spixfer((&p_header->msg_type)+1, sizeof(sdepMsgHeader_t) - 1);

        // Command is 16-bit at odd address, may have alignment issue with 32-bit chip
        uint16_t cmd_id = word(p_header->cmd_id_high, p_header->cmd_id_low);

        // Error Message Response
        if ( p_header->msg_type == SDEP_MSGTYPE_ERROR ) break;

        // Invalid command
        if (!(cmd_id == SDEP_CMDTYPE_AT_WRAPPER ||
              cmd_id == SDEP_CMDTYPE_BLE_UARTTX ||
              cmd_id == SDEP_CMDTYPE_BLE_UARTRX) )
        {
            break;
        }

        // Invalid length
        if(p_header->length > SDEP_MAX_PACKETSIZE) break;

        // read payload
        memset(p_response->payload, 0xff, p_header->length);
        spixfer(p_response->payload, p_header->length);

        result = true;

    }while(0);

    SPI_CS_DISABLE();

    spi.end();

    return result;
}

}
