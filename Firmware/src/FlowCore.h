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

#define VERSION "0.1.0" // Prototype firmware version

// Included Libraries
#include <Arduino.h> // Standard Arduino libraries.
#include <AccelStepper.h> // For motors
#include <PID_v1.h> // For PID control
#include <PID_AutoTune_v0.h> // For PID autotuning
#include <Thermocouple.h> // For the thermocouple
#include <MAX6675_Thermocouple.h> // Also for the thermocouple
#define USE_TIMER_2     1
#include "TimerInterrupt.h" // For interrupts

#include "HAL/megaatmega2560/megaatmega2560.h"
#include "HAL/megaatmega2560/serial.h"
#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "hardware/heater.h"
#include "hardware/motor.h"
#include "hardware/tmc5160.h"

// Gcode objects
gcode gcodeHandler;
parser parserHandler;

// Motor objects
Motor motorHandler;

// All checkups are done within this function.
void idle();
