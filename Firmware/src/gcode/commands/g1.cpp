#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Starts the given motor with the given speed
void gcode::g1(GcodeCommand_t cmd){
  Motor motorHandler; // TODO: Should this be creating this every time?
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  GcodeArg_t speed = gcodeParseValueFor('s', cmd);
  if (motorHandler.isValidMotor(motor)) {
    Serial.println(F("Starting a motor..."));
    motorHandler.set_speed(motor, speed);
    motorHandler.start(motor);
  } else {
    Serial.println(F("Invalid motor number entered."));
  }
} // gcode::g1
