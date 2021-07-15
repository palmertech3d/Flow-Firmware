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
  gcodeHandler.get_gcode();
  gcodeHandler.execute_buffer();
  Heater::update();
}


/* FIRMWARE ENTRY POINT */
void setup() {

  // Setting output for fan, heater, and winder limit switch
  SET_OUTPUT(FAN);
  SET_OUTPUT(HEATER);
  SET_INPUT(WIND_LIM_SWITCH);
  SET_INPUT_PULLUP(WIND_LIM_SWITCH);
  SET_OUTPUT(EXTRUDER_CS);

  // Configure the hotend
  Heater::init(0);
  Heater::set_constants(10, 0.5, 5);

  // Init timer ITimer2 for stepper interrupts
  ITimer2.init();
  #define TIMER2_INTERVAL_MS 1
  ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, Motor::run);

  // Init the tmc5160 driver for the extruder motor
  TMC5160::init();

  Serial.begin(9600);

  Serial.print(F("Flow Extruder MK1 running firmware version ")); Serial.println(VERSION);
  Serial.println(F("Type a gcode command."));
}


/* THE MAIN LOOP */
void loop() {
  idle();
  _delay_ms(250);
}
