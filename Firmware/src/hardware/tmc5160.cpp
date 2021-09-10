#include "tmc5160.h"

uint32_t buildChopperConfig() {
  uint32_t config = 0;
  config &= ~(COPCONF_CHM_CHOP_MODE);

  config &= ~(COPCONF_HEND_MASK);
  config |= COPCONF_HEND_MASK & (1ul << COPCONF_HEND_SHIFT);

  config &= ~(COPCONF_HSTRT_MASK);
  config |= COPCONF_HSTRT_MASK & (4ul << COPCONF_HSTRT_SHIFT);

  config &= ~(COPCONF_TOFF_MASK);
  config |= COPCONF_TOFF_MASK & (3ul << COPCONF_TOFF_SHIFT);

  config &= ~(COPCONF_FD3);

  config &= ~(COPCONF_DISFDCC_CHOP_MODE_FAST_DECAY_DISABLE_COMPARATOR);

  config &= ~(COPCONF_TIME_BLANK_MASK);
  config |= COPCONF_TIME_BLANK_MASK & (2ul << COPCONF_TIME_BLANK_SHIFT);

  config &= ~(COPCONF_VHIGHFS_HIGH_VEL_FULLSTEP);

  config &= ~(COPCONF_VHIGHCHM_HIGH_VEL_CHOP);

  config &= ~(COPCONF_TPFD_MASK);

  config &= ~(COPCONF_USTEP_RESOLTN_MASK);
  config |= COPCONF_USTEP_RESOLTN_MASK & (8ul << COPCONF_USTEP_RESOLTN_SHIFT);

  config &= ~(COPCONF_SHORT_TO_SUPPLY_DISABLE);
  config &= ~(COPCONF_SHORT_TO_GND_DISABLE);
  config &= ~(COPCONF_DOUBLE_EDGE_EN);
  config &= ~(COPCONF_USTEP_256);

  return config;
} // sendChopperConfig

void TMC5160::init(){
  digitalWrite(PIN_EXTRUDER_CS, HIGH);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();

  sendData(REG_COPCONF, buildChopperConfig());
  sendData(REG_COOLCONF, 0x3F0000);
  sendData(REG_IHOLD_IRUN, 0x6190A);
  sendData(REG_TPOWERDOWN, 0xA);
  sendData(REG_GCONF, 0x4);
  sendData(REG_TPWM_THRS, 0x1F4);
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
