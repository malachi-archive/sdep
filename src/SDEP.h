#pragma once

#include "adafruit/sdep.h"
#include "spi_shim.h"

#include <mbed.h>

#ifndef ARDUINO
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#endif

// TODO: refactor naming
namespace FactUtilEmbedded
{

template <class TSPI>
class SDEP
{
    SPI<TSPI> spi;

    // MBED specific, need to iron this out
    // +++
    DigitalIn irq;
    // ---
public:
    template <class ...TArgs>
    SDEP(PinName pin_irq, TArgs...args) : spi(args...), irq(pin_irq)
    {
    }

    void test()
    {
        spi.transfer(10);
    }

    void sendPacket(uint16_t command, const uint8_t* buf, uint8_t count, uint8_t more_data);
    bool getPacket(sdepMsgResponse_t* p_response);
    bool irq_pin();
};

}
