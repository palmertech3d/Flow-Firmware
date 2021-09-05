#include "tmc5160.h"

void TMC5160::init(){
  digitalWrite(PIN_EXTRUDER_CS, HIGH);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();

  sendData(0xEC, 0x80100C3); // CHOPCONF
  sendData(0xED, 0x3F0000); // COOLCONF
  sendData(0x90, 0x6190A); // IHOLD_IRUN
  sendData(0x91, 0xA); // TPOWERDOWN
  sendData(0x80, 0x4); // GCONF
  sendData(0x93, 0x1F4); // TPWM_THRS
} // TMC5160::init

unsigned long TMC5160::sendData(unsigned long address, unsigned long datagram)
{
  // TMC5130 takes 40 bit data: 8 address and 32 data
  unsigned long i_datagram = 0;
  digitalWrite(PIN_EXTRUDER_CS, LOW);
  delayMicroseconds(10);

  SPI.transfer(address);

  i_datagram |= SPI.transfer((datagram >> 24) & 0xff);
  i_datagram <<= 8;
  i_datagram |= SPI.transfer((datagram >> 16) & 0xff);
  i_datagram <<= 8;
  i_datagram |= SPI.transfer((datagram >> 8) & 0xff);
  i_datagram <<= 8;
  i_datagram |= SPI.transfer((datagram) & 0xff);
  digitalWrite(PIN_EXTRUDER_CS, HIGH);

  return i_datagram;
} // TMC5160::sendData
