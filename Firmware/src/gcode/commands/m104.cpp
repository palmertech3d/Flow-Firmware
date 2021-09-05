#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/heater.h"

void gcode::m104(GcodeCommand_t cmd){
  GcodeArg_t temp = gcodeParseValueFor('s', cmd);
  Serial.print(F("Setting hotend temp to ")); Serial.print(temp); Serial.println(F(" C"));
  Heater::set((double)temp);
} // gcode::m104
