#include <mbed.h>
#include <mbed_events.h>

#include <SDEP.h>
//#include <SPI.h>

namespace util = FactUtilEmbedded;

#define PIN_IRQ NC
#define PIN_CS NC

#if SAM
#define MAL_LED PA17
#else
#define MAL_LED LED1
// ST303K8 pinouts
#define MOSI PA_7
#define MISO PA_6
#define SCK PA_5

// NOTE: definitions for STM303K8 don't line up with what I used...
// surprised mine works... perhaps lines are somewhat reconfigurable?
//#define MOSI SPI_MOSI
//#define MISO SPI_MISO
//#define SCK SPI_SCK
#endif

util::hal::SPI spi(MOSI, MISO, SCK);


static void blinky(void) {
    static DigitalOut led(MAL_LED);
    led = !led;
    printf("LED = %d \r\n",led.read());
}


static void spi_send()
{
    char buf[] = "\1\2\3\4";

    spi.write(buf, 4);

    spi.transfer(5);

    /*
    spi.transfer(1);
    spi.transfer(2);
    spi.transfer(3);
    spi.transfer(4);
    spi.transfer(5); */
}

int main()
{
    EventQueue queue;

    //sdep.sendPacket(0, (const uint8_t*)"hello", 6, 0);

    // TODO: See if linker will take it easy on us and not try to link un functions
    // whose static if here never resolves to true
    if(util::hal::SPI::traits_t::async())
    {

    }

    queue.call_in(2000, printf, "called in 2 seconds!");
    queue.call_every(1000, blinky);
    queue.call_every(5000, spi_send);

    queue.dispatch();
}
