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

// TODO: break this into two private transfer functions:
// transfer_tx_greater and transfer_rx_greater as it will optimize to faster and probably
// smaller code
template <>
void SPI< ::SPI>::transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length)
{
#if DEVICE_SPI_ASYNCH_XX
    // TODO: Still need to work out callback behavior, so this code is not yet enabled
#else
    size_t min_length = min(tx_length, rx_length);
    auto in = (uint8_t*) rx_buffer;
    auto out = (uint8_t*) tx_buffer;

    while(min_length--)
    {
        *in = transfer(*out);
        in++;
        out++;
    }

    tx_length -= min_length;
    rx_length -= min_length;

    while(tx_length--)
        transfer(*out++);

    while(rx_length--)
        *in++ = transfer(0);

#endif
}

}



#endif
