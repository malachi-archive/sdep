#pragma once

#include <SPI.h>

namespace FactUtilEmbedded
{
    
template <class TSPI>
class spi_context;

namespace experimental
{

template <class TSPI>
class spi_base;

}

template <class TSPI>
class spi_traits
{
public:
    static bool can_set_bitwidth();
    static bool can_set_dataorder();
    
    static bool async(); // does this SPI devie support async API
};




#ifdef __MBED__

template <>
class spi_context<mbed::SPI>
{
    
};

template <>
class spi_traits<mbed::SPI>
{
public:
    static bool can_set_bitwidth() { return true; }
    static bool can_set_dataorder() { return false; }
    
    static bool async() { return DEVICE_SPI_ASYNCH; }
};


#elif defined(ARDUINO)

// UNTESTED CODE
template <>
class spi_context< ::SPI> : public SPISettings
{
    
};

template <>
class spi_traits< ::SPI>
{
public:
    static bool can_set_bitwidth() { return true; }
    static bool can_set_dataorder() { return true; }
    
    static bool async() { return false; }
};


#endif

}
