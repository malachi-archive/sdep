#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "spi_shim_context.h"


namespace FactUtilEmbedded
{

enum spi_modes
{
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3
};

template <class TSPI>
class SPI
{
    TSPI spi;
    spi_context<TSPI> context;

public:
    typedef spi_traits<TSPI> traits;

    int transfer(uint16_t value);
    void transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length);

    template <class ...TArgs>
    SPI(TArgs...args) : spi(args...)
    {

    }

    TSPI& native() { return spi; }
};

}
