#pragma once

#include <stdint.h>
#include <stdlib.h>

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
class spi_traits
{
public:
    static bool can_set_bitwidth();
    static bool can_set_dataorder();
    
    static bool async(); // does this SPI devie support async API
};


template <class TSPI>
class spi_context;

template <class TSPI>
class SPI
{
    TSPI spi;

public:
    typedef spi_traits<TSPI> traits;

    uint16_t transfer(uint16_t value);
    void transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length);

    template <class ...TArgs>
    SPI(TArgs...args) : spi(args...)
    {

    }

    TSPI& native() { return spi; }
};

}
