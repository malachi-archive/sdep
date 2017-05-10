#include "SDEP.cpp"

// technique outlined here for non-inlining template functions https://isocpp.org/wiki/faq/templates

#ifdef __MBED__
#include <mbed.h>

template class FactUtilEmbedded::SDEP<SPI>;

#elif defined(ARDUINO)
#include <Arduino.h>

template class FactUtilEmbedded::SDEP<SPI>;

#endif
