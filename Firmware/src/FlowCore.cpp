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
  gcode_handler.getGcode();
  gcode_handler.executeBuffer();
  Heater::update();
  motor_handler.idle();
  tabular_delegator.idle();
} // idle

/* FIRMWARE ENTRY POINT */
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  // Setting output for fan, heater, and winder limit switch
  SET_OUTPUT(PIN_FAN);
  SET_OUTPUT(PIN_HEATER);
  SET_INPUT(PIN_LIM_SWITCH_WINDER);
  SET_INPUT_PULLUP(PIN_LIM_SWITCH_WINDER);
  SET_OUTPUT(PIN_EXTRUDER_CS);

  // Configure the hotend
  Heater::init();
  Heater::setConstants(10, 0.5, 5);

  // Init timer ITimer2 for stepper interrupts
  ITimer2.init();
  ITimer2.attachInterruptInterval(1, Motor::run); // 1 ms

  // Init the tmc5160 driver for the extruder motor
  TMC5160::init();

  LOG_INFO(F("Flow Extruder MK1 running firmware version " VERSION " \n"));
  LOG_INFO(F("Type a gcode command.\n"));

  #ifdef UNIT_LEVEL_TESTING
  runUnitTestsThenLoop();
  #endif // ifdef UNIT_LEVEL_TESTING
} // setup

/* THE MAIN LOOP */
void loop() {
  idle();
  // _delay_ms(250);
} // loop
