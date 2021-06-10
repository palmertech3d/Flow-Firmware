/**
 * Palmer Technologies Flow Extruder Firmware
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

#define VERSION "1.0" // Prototype firmware version


// Included Libraries
#include <Arduino.h> // Standard Arduino libraries. Will be ditched after HAL is completed
#include "HAL/megaatmega2560/megaatmega2560.h"
#include "HAL/megaatmega2560/serial.h"
#include "gcode/gcode.h" // gcode library
#include "gcode/parser.h" // parser library

#include <AccelStepper.h> // For motors
#include <Encoder.h> // For rotary encoder
#include <PID_v1.h> // For PID control
#include <PID_AutoTune_v0.h>
#include <Thermocouple.h>          // Thermocouple libraries
#include <MAX6675_Thermocouple.h>  //
//==================================

// Function Declarations

void blankFunction(); // Used for attaching to LiquidLines to make them focusable, and thus scrollable
void returnPIDConstants(); // Displays constants for the PID calculated by the PID AutoTune library

//==================================


//// GLOBAL OBJECTS & VARIABLES


// Stepper objects
AccelStepper m_extruder(1, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper m_roller(1, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper m_winder(1, M_WINDER_STEP, M_WINDER_DIR);
//==================================


// Thermocouple Object
Thermocouple* thermocouple = new MAX6675_Thermocouple(THERMO_SCK, THERMO_CS, THERMO_SO);
//==================================


// PID object & variables
double temperature = -99; // Used to display the temperature to serial
double newTemp = 0; // A temporary temperature storage place used when reading the thermocouple
double setTemp = 0;
double output;
double Kp = 2.16, Ki =0.08 , Kd = 15.28;
unsigned long pidTimekeeper = 0;
PID pid(&temperature, &output, &setTemp, Kp, Ki, Kd, DIRECT);
//==================================


// PID AutoTune setup
PID_ATune pidAuto(&temperature, &output);
bool stopPidAuto = false; // Set to true when the autotuner finishes
//==================================


// Motor variables
bool levelHomed = false; // Set to true when level winder has been homed

short extruderMotorStatus = 0; // The speed of the stepper motors in rpm
short rollerMotorStatus = 0;
short levelMotorStatus = 0;
short winderMotorStatus = 0;

//==================================

char *buffer = (char *)malloc(sizeof(char) * 6); // Buffer for serial input
gcode gcodeHandler;
<<<<<<< HEAD
parser* parserHandler = new parser();
=======
parser parserHandler;
>>>>>>> 156697314cddd2ad98465479b04dbcb4eca28b9e

//// SETUP FUNCTION

void setup() {
  // Setting output for fan, heater, and winder limit switch
  SET_OUTPUT(FAN);
  SET_OUTPUT(HEATER);
  SET_INPUT(WIND_LIM_SWITCH);
  SET_INPUT_PULLUP(WIND_LIM_SWITCH);


  // Begin initial device setup
  //gcodeHandler.G28(); // Home winder

  // Start PID
  pid.SetMode(AUTOMATIC);

  // PID AutoTune
  pidAuto.SetControlType(1);

  // Stepper motor setup
  m_extruder.setMaxSpeed(1000);
  m_extruder.setSpeed(200);

  m_roller.setMaxSpeed(1000);
  m_roller.setSpeed(200); // SAMPLE SPEED!!!!

  m_winder.setMaxSpeed(1000);
  m_winder.setSpeed(50);

  // Serial setup for communication with PC
  usart0_init(9600);
  usart0_write("Flow Extruder MK1 running firmware version 1.0.\r\n");
  usart0_write("Type a gcode command to start.\r\n> ");

}




//// LOOP FUNCTION
void loop() {
  gcodeHandler.get_gcode();
<<<<<<< HEAD
  usart0_write_int(parserHandler->parsegcode("G12"));
  
  _delay_ms(5000);
=======
  _delay_ms(1000);
>>>>>>> 156697314cddd2ad98465479b04dbcb4eca28b9e

  usart0_write_str("\r\n> ");


}



 //// void returnPIDConstants()

 void returnPIDConstants(){
   Serial.print("PID Kp = ");
   Serial.println(pidAuto.GetKp());
   Serial.print("PID Ki = ");
   Serial.println(pidAuto.GetKi());
   Serial.print("PID Kd = ");
   Serial.println(pidAuto.GetKd());
   return;
 }
