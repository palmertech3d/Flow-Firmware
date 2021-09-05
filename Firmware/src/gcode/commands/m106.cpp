#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>

// M106 turns cooling fans on/off
void gcode::m106(GcodeCommand_t cmd){
  Serial.println(F("Toggling cooling fans"));
  TOGGLE(PIN_FAN);
} // gcode::m106
