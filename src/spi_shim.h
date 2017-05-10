#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace FactUtilEmbedded
{

template <class TSPI>
class SPI
{
    TSPI spi;

public:
    uint16_t transfer(uint16_t value);
    void transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length);

    template <class ...TArgs>
    SPI(TArgs...args) : spi(args...)
    {

    }

    TSPI& native() { return spi; }
};

}
