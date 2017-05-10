#pragma once

#include "sdep.h"
#include "spi_shim.h"

// TODO: refactor naming
namespace FactUtilEmbedded
{

template <class TSPI>
class SDEP
{
    SPI<TSPI> spi;

public:
    template <class ...TArgs>
    SDEP(TArgs...args) : spi(args...)
    {
    }

    void test()
    {
        spi.transfer(10);
    }
};

}
