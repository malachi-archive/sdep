#ifdef __MBED__

#include "spi_shim.h"
#include <mbed.h>
#include <SPI.h>


namespace FactUtilEmbedded
{

template <>
uint16_t SPI< ::SPI>::transfer(uint16_t value)
{
    return spi.write(value);
}

template <>
void SPI< ::SPI>::transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length)
{

}

}



#endif
