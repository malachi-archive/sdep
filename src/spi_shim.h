#pragma once

#include <stdint.h>

namespace FactUtilEmbedded
{

template <class TSPI>
class SPI
{
    TSPI spi;

public:
    uint16_t transfer(uint16_t value);

    template <class ...TArgs>
    SPI(TArgs...args) : spi(args...)
    {

    }
};

}
