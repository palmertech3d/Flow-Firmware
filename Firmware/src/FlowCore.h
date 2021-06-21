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

#define VERSION "1.0" // Prototype firmware version

// Included Libraries
#include <Arduino.h> // Standard Arduino libraries.
#include <AccelStepper.h> // For motors
#include <PID_v1.h> // For PID control
#include <PID_AutoTune_v0.h>
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>

#include "HAL/megaatmega2560/megaatmega2560.h"
#include "HAL/megaatmega2560/serial.h"
#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "hardware/heater.h"

//// GLOBAL OBJECTS & VARIABLES

// Stepper objects
AccelStepper m_extruder(1, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper m_roller(1, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper m_winder(1, M_WINDER_STEP, M_WINDER_DIR);
//==================================


// Hotend object
Heater hotend;


// Motor variables
bool levelHomed = false; // Set to true when level winder has been homed

short extruderMotorStatus = 0; // The speed of the stepper motors in rpm
short rollerMotorStatus = 0;
short levelMotorStatus = 0;
short winderMotorStatus = 0;

//==================================

char *buffer = (char *)malloc(sizeof(char) * 6); // Buffer for serial input
gcode gcodeHandler;
parser parserHandler;
