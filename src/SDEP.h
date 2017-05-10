#pragma once

#include "adafruit/sdep.h"
#include "spi_shim.h"

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

public:
    template <class ...TArgs>
    SDEP(TArgs...args) : spi(args...)
    {
    }

    void test()
    {
        spi.transfer(10);
    }

    void sendPacket(uint16_t command, const uint8_t* buf, uint8_t count, uint8_t more_data);
};

}
