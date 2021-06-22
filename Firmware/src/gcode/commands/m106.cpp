#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>

// M106 turns cooling fans on/off
void gcode::m106(){
  Serial.println("Toggling cooling fans");
  TOGGLE(FAN);
}
