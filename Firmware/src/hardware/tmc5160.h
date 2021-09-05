#ifndef TMC5160_H
#define TMC5160_H

#include <SPI.h>
#include "../HAL/megaatmega2560/megaatmega2560.h"

class TMC5160 {
public:

static void init();

private:

static unsigned long sendData(unsigned long address, unsigned long datagram);
}; // class TMC5160

#endif // TMC5160_H
