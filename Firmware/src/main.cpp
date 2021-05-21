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
void homeLevelWinder(); // Call to home the level winder; this function will delay all future action until the winder has been homed

//==================================


//// GLOBAL OBJECTS & VARIABLES


// Stepper objects
AccelStepper m_extruder(1, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper m_roller(1, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper m_level(1, M_LEVEL_STEP, M_LEVEL_DIR);
AccelStepper m_winder(1, M_WINDER_STEP, M_WINDER_DIR);
//==================================


// Thermocouple Object
Thermocouple* thermocouple = new MAX6675_Thermocouple(THERMO_SCK, THERMO_CS, THERMO_SO);
//==================================


// PID object & variables
double temperature = -99; // Used to display the temperature on the LCD
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

char *buffer = (char *)malloc(sizeof(char) * 6);
//// SETUP FUNCTION

void setup() {

  // Start serial monitor for debugging purposes
  Serial.begin(9600);

  // General pinmodes
  pinMode(FAN, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(WIND_LIM_SWITCH, INPUT);

  // Begin initial device setup
  //homeLevelWinder(); // Home the level winder; this function will delay all future action until the winder has been homed

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
  //Serial.println("Flow Extruder MK1 running firmware version 1.0.");
  //Serial.println("Type a gcode command.");

  //usart0_init();
}



char someChar = 0;
//// LOOP FUNCTION
void loop() {
  //HAL::blinkLED();

  /*
  if (Serial.available() > 0){
    Serial.readBytesUntil(13, buffer, 20);
    Serial.print("Echo: ");
    //Serial.println(buffer);
    //*buffer = NULL;

    for (int i = 0; i < sizeof(buffer); i++){
      Serial.print(buffer[i]);
    }
    Serial.println("");
    ++*buffer = 0;

  }
  */

  /*
  if(Serial.available() > 0 && Serial.read() !=13 ){
    int i = 0;

    while(Serial.read() != 13){
      if (Serial.available() > 0){
        buffer[i] = Serial.read();
        i++;
        Serial.print("Going!");
      }
    }

    for (int i = 0; i < sizeof(buffer); i++){
      Serial.print(buffer[i]);
    }

  }

*/
//someChar = usart0_receive();
//usart0_transmit('a');
Serial.println(F_CPU);
_delay_ms(1000);

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

 // Homes the level winder; this function will delay all future action until the winder has been homed
 void homeLevelWinder(){

   // Get level winder ready to be homed
   m_level.setCurrentPosition(0);
   m_level.moveTo(5000); // Get the level winder ready to move fast towards the limit switch
   m_level.setMaxSpeed(1000);
   m_level.setSpeed(500);
   m_level.setAcceleration(1000);

   while(!levelHomed){
     m_level.run(); // Move the level winder towards the limit switch

     if(digitalRead(WIND_LIM_SWITCH) == LOW){
       levelHomed = true; // Let us know that the level has been homed
       m_level.setCurrentPosition(0); // Set our starting pos to 0
       m_level.moveTo(-1500); // Move the level winder out from the limit switch

     }
   }

   while(m_level.currentPosition() != -1500){
     m_level.run();
     }
 }
