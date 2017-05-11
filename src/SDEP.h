#pragma once

#include "adafruit/sdep.h"
#include "spi_shim.h"

#include <mbed.h>

#ifndef ARDUINO
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

inline unsigned int makeWord(unsigned int w) { return w; }
inline unsigned int makeWord(unsigned char h, unsigned char l) { return (h << 8) | l; }

#define word(...) makeWord(__VA_ARGS__)

#endif

// TODO: refactor naming
namespace FactUtilEmbedded
{

template <class TSPI>
class SDEP
{
    SPI<TSPI> spi;

    // MBED specific, need to iron this out
    // +++
    DigitalIn irq;
    DigitalOut cs;
    // ---
public:
    template <class ...TArgs>
    SDEP(PinName pin_irq, PinName pin_cs, TArgs...args) : spi(args...), irq(pin_irq), cs(pin_cs)
    {
    }

    void test()
    {
        spi.transfer(10);
    }

    bool sendPacket(uint16_t command, const uint8_t* buf, uint8_t count, uint8_t more_data);
    bool getPacket(sdepMsgResponse_t* p_response);
    bool irq_pin();
    void cs_enable() { cs = 0; }
    void cs_disable() { cs = 1; }

    void spixfer(void* buf, uint16_t count);
    void flush();
};

}
