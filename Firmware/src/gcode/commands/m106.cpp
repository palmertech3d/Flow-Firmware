#ifndef M106_H
#define M106_H

#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>

// M106 turns cooling fans on/off
// Toggles fans
void gcode::m106(){
  TOGGLE(FAN);
}

#endif // M106_H
