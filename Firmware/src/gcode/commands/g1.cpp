#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Starts the given motor with the given speed
void GcodeExecuter::g1(GcodeCommand_t cmd){
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  GcodeArg_t speed = gcodeParseValueFor('s', cmd);
  if(motor == GCODE_ARG_VALUE_ERR) motor = -1; // Set to invalid motor number
  if(speed == GCODE_ARG_VALUE_ERR) {
    LOG_WARN("Motor speed invalid; setting to zero\n");
    speed = 0;
    }
  if (Motor::isValidMotor(motor)) {
    LOG_INFO(F("Setting motor speed\n"));
    Motor::setSpeed(motor, speed);
    Motor::start(motor);
  } else {
    LOG_WARN("Invalid motor number entered.\n");
  }
} // GcodeExecuter::g1
