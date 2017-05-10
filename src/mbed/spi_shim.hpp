// designed to ONLY be included from spi_shim.  Don't include otherwise!
template <>
inline void SPI<mbed::SPI>::format(uint8_t bits, spi_modes mode)
{
    spi.format(bits, mode);
}

template <>
inline void SPI<mbed::SPI>::frequency(int32_t hz)
{
    spi.frequency(hz);
}
