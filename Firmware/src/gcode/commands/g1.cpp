#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Starts the given motor with the given speed
void GcodeExecuter::g1(GcodeCommand_t cmd){
  Motor motor_handler; // TODO: Should this be creating this every time?
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  GcodeArg_t speed = gcodeParseValueFor('s', cmd);
  if (motor_handler.isValidMotor(motor)) {
    Serial.println(F("Starting a motor..."));
    motor_handler.set_speed(motor, speed);
    motor_handler.start(motor);
  } else {
    Serial.println(F("Invalid motor number entered."));
  }
} // GcodeExecuter::g1
