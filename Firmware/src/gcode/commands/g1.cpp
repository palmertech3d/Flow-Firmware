#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Starts the given motor with the given speed
void GcodeExecuter::g1(GcodeCommand_t cmd){
  Motor motor_handler; // TODO: Should this be creating this every time?
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  GcodeArg_t speed = gcodeParseValueFor('s', cmd);
  if(motor == GCODE_ARG_VALUE_ERR) motor = -1; // Set to invalid motor number
  if(speed == GCODE_ARG_VALUE_ERR) speed = 0;
  if (motor_handler.isValidMotor(motor)) {
    LOG_INFO(F("Setting motor speed\n"));
    motor_handler.set_speed(motor, speed);
    motor_handler.start(motor);
  } else {
    LOG_INFO(F("Invalid motor number entered.\n"));
  }
} // GcodeExecuter::g1
