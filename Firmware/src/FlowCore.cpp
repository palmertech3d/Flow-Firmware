/**
 * Palmer Technologies LLC Flow Extruder Firmware
 * Copyright (c) 2021 palmertech3d [https://github.com/palmertech3d/Flow-Firmware]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


#include "FlowCore.h"

void idle(){
  //Serial.print(F("Free memory: ")); Serial.println(freeMemory()); // Print amount of free memory remaining
  gcodeHandler.get_gcode();
  gcodeHandler.execute_buffer();
  Heater::update();
}

unsigned long sendData(unsigned long address, unsigned long datagram)
{
  //TMC5130 takes 40 bit data: 8 address and 32 data
  unsigned long i_datagram = 0;
  digitalWrite(EXTRUDER_CS, LOW);
  delayMicroseconds(10);

  SPI.transfer(address);

  i_datagram |= SPI.transfer((datagram >> 24) & 0xff);
  i_datagram <<= 8;
  i_datagram |= SPI.transfer((datagram >> 16) & 0xff);
  i_datagram <<= 8;
  i_datagram |= SPI.transfer((datagram >> 8) & 0xff);
  i_datagram <<= 8;
  i_datagram |= SPI.transfer((datagram) & 0xff);
  digitalWrite(EXTRUDER_CS, HIGH);

  Serial.print("Received: ");
  Serial.println(i_datagram,HEX);
  Serial.print(" from register: ");
  Serial.println(address,HEX);

  return i_datagram;
}

/* FIRMWARE ENTRY POINT */

void setup() {

  // Setting output for fan, heater, and winder limit switch
  SET_OUTPUT(FAN);
  SET_OUTPUT(HEATER);
  SET_INPUT(WIND_LIM_SWITCH);
  SET_INPUT_PULLUP(WIND_LIM_SWITCH);
  SET_OUTPUT(EXTRUDER_CS);
  digitalWrite(EXTRUDER_CS, HIGH);

  // Configure the hotend
  Heater::init(0);
  Heater::set_constants(10, 0.5, 5);

  // Init timer ITimer2 for stepper interrupts
  ITimer2.init();
  #define TIMER2_INTERVAL_MS 1
  ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, Motor::run);

  Serial.begin(9600);

  // TMC5160 driver init
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();

  /*
   sendData(0x89,0x00010606);      // SHORTCONF
   sendData(0x8A,0x00080400);      // DRV_CONF
   sendData(0x90,0x00080303);      // IHOLD_IRUN
   sendData(0x91,0x0000000A);      // TPOWERDOWN
   sendData(0xAB,0x00000001);      // VSTOP
   sendData(0xBA,0x00000001);      // ENC_CONST
   sendData(0xEC,0x15410153);      // CHOPCONF
   sendData(0xF0,0xC40C001E);      // PWMCONF
  */

  sendData(0xEC, 0x80100C3); // CHOPCONF
  sendData(0xED, 0x3F0000); // COOLCONF
  sendData(0x90, 0x61F0A); // IHOLD_IRUN
  sendData(0x91, 0xA); // TPOWERDOWN
  sendData(0x80, 0x4); // GCONF
  sendData(0x93, 0x1F4); // TPWM_THRS

  Serial.print(F("Flow Extruder MK1 running firmware version ")); Serial.println(VERSION);
  Serial.println(F("Type a gcode command."));
}



/* THE MAIN LOOP */

char test = 'a';

void loop() {
  idle();
  _delay_ms(250);
}
