#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Stops the given motor
void GcodeExecuter::g2(GcodeCommand_t cmd){
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  Motor motor_handler;
  if (motor_handler.isValidMotor(motor)) {
    Serial.println(F("Stopping a motor..."));
    motor_handler.stop(motor);
  }
} // GcodeExecuter::g2
