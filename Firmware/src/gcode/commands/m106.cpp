#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>

// M106 turns cooling fans on/off
void GcodeExecuter::m106(GcodeCommand_t cmd){
  LOG_INFO(F("Toggling cooling fans"));
  TOGGLE(PIN_FAN);
} // GcodeExecuter::m106
