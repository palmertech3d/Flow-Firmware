#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/heater.h"

void GcodeExecuter::m104(GcodeCommand_t cmd){
  GcodeArg_t temperature = gcodeParseValueFor('s', cmd);
  if(temperature == GCODE_ARG_VALUE_ERR) temperature = 0;
  LOG_INFO(F("Setting hotend temperature to ")); LOG_INFO(temperature); LOG_INFO(F(" C\n"));
  Heater::set((double)temperature);
} // GcodeExecuter::m104
