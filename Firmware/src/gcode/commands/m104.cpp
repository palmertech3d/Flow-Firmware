#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/heater.h"

// m104 sets the target temperature of the hotend
void gcode::m104(){
  Serial.println("Turning off the hotend...");
  Heater::set(0);
}


void gcode::m104(int temp){
  Serial.print("Setting hotend temp to "); Serial.print(temp); Serial.println(" C");
  Heater::set((double)temp);
}
