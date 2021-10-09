#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

/// Stops the given motor
void GcodeExecuter::g2(GcodeCommand_t cmd){
  GcodeArg_t motor = gcodeParseValueFor('m', cmd);
  if (Motor::isValidMotor(motor)) {
    LOG_INFO(F("Stopping a motor...\n"));
    Motor::stop(motor);
  }
} // GcodeExecuter::g2
