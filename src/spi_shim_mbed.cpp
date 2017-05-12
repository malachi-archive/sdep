#ifdef __MBED__

#include "spi_shim.h"
#include <mbed.h>
#include <algorithm>


namespace FactUtilEmbedded
{

template <>
int SPI< ::SPI>::transfer(uint16_t value)
{
    return spi.write(value);
}


inline static void transfer(SPI< ::SPI>& spi, const uint8_t*& tx_buffer, uint8_t*& rx_buffer, size_t length)
{
    while(length--)
    {
        *rx_buffer = spi.transfer(*tx_buffer);
        tx_buffer++;
        rx_buffer++;
    }
}

// transfer buffers where tx buffer is larger
static void xfer_more_tx(SPI< ::SPI>& spi, const uint8_t* tx_buffer, size_t tx_length, uint8_t* rx_buffer, size_t rx_length)
{
    tx_length -= rx_length;

    transfer(spi, tx_buffer, rx_buffer, rx_length);

    while(tx_length--)
        spi.transfer(*tx_buffer++);
}

// transfer buffers where rx buffer is larger
static void xfer_more_rx(SPI< ::SPI>& spi, const uint8_t* tx_buffer, size_t tx_length, uint8_t* rx_buffer, size_t rx_length)
{
    rx_length -= tx_length;

    transfer(spi, tx_buffer, rx_buffer, tx_length);

    while(rx_length--)
        *rx_buffer++ = spi.transfer(0);
}


template <>
void SPI< ::SPI>::transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length)
{
#if DEVICE_SPI_ASYNCH_XX
    // TODO: Still need to work out callback behavior, so this code is not yet enabled
#else
    if(tx_length > rx_length)
        xfer_more_tx(*this, (const uint8_t*)tx_buffer, tx_length, (uint8_t*)rx_buffer, rx_length);
    else
        xfer_more_rx(*this, (const uint8_t*)tx_buffer, tx_length, (uint8_t*)rx_buffer, rx_length);
#endif
}


}



#endif
