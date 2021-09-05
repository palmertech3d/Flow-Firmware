#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/heater.h"
#include <Arduino.h>

// m303 activates PID auto tune for the hotend
void gcode::m303(){
  Serial.println(F("Starting autotune for hotend..."));
  Heater::autotune_init();
} // gcode::m303
