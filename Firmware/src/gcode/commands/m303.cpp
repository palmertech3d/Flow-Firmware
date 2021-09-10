#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/heater.h"
#include <Arduino.h>

// m303 activates PID auto tune for the hotend
void GcodeExecuter::m303(GcodeCommand_t cmd){
  INFO_LOG(F("Starting autotune for hotend..."));
  Heater::autotune_init();
} // GcodeExecuter::m303
