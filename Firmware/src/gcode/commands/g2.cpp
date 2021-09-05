#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Stops the given motor
void gcode::g2(GcodeCommand_t cmd){
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  Motor motorHandler;
  if (motorHandler.isValidMotor(motor)) {
    Serial.println(F("Stopping a motor..."));
    motorHandler.stop(motor);
  }
} // gcode::g2
