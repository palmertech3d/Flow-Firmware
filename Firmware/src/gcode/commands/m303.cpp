#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/heater.h"
#include <Arduino.h>

// m303 activates PID auto tune for the hotend
void GcodeExecuter::m303(GcodeCommand_t cmd){
  GcodeArg_t target = gcodeParseValueFor('s', cmd);
  GcodeArg_t band = gcodeParseValueFor('b', cmd);
  if (target == GCODE_ARG_VALUE_ERR || band == GCODE_ARG_VALUE_ERR) {
    LOG_INFO(F("Stopping heater autotune\n"));
    Heater::autotuneStop();
    return;
  }
  LOG_INFO(F("Starting heater autotune\n"));
  Heater::autotune_init(target, band);
} // GcodeExecuter::m303
