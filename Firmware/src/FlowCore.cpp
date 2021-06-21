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


/* FIRMWARE ENTRY POINT */

void setup() {


  // Setting output for fan, heater, and winder limit switch
  SET_OUTPUT(FAN);
  SET_OUTPUT(HEATER);
  SET_INPUT(WIND_LIM_SWITCH);
  SET_INPUT_PULLUP(WIND_LIM_SWITCH);

  // Configure the hotend
  hotend.init(200);
  //hotend.set_constants(2.16, 0.08, 15.28);
  hotend.set_constants(0.25, 0, 11.66211);
  //hotend.autotune_init();


  // Stepper motor setup
  m_extruder.setMaxSpeed(1000);
  m_extruder.setSpeed(200);

  m_roller.setMaxSpeed(1000);
  m_roller.setSpeed(200); // SAMPLE SPEED!!!!

  m_winder.setMaxSpeed(1000);
  m_winder.setSpeed(50);

  // Serial setup for communication with PC
  usart0_init(9600);
  //usart0_write("Flow Extruder MK1 running firmware version 1.0.\r\n");
  //usart0_write("Type a gcode command to start.\r\n> ");
}



/* THE MAIN LOOP */

void loop() {

  //gcodeHandler.get_gcode();

  //gcodeHandler.execute_buffer();
  hotend.update_heater();

  _delay_ms(2000);

  //usart0_write_str("\r\n> ");


}
