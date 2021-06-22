#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>
#include "../../hardware/heater.h"

// M503 reports settings to the terminal
void gcode::m503(){
  Serial.println("Current settings for Flow Extruder:");
  Serial.print("Current hotend temp: "); Serial.println((int)Heater::get());
  Serial.print("Target hotend temp: "); Serial.println((int)Heater::get_target());
  double constants[3];
  Heater::get_constants(constants);
  Serial.println("Hotend PID constants: ");
  Serial.print("\tKp: "); Serial.println(constants[0]);
  Serial.print("\tKi: "); Serial.println(constants[1]);
  Serial.print("\tKd: "); Serial.println(constants[2]);
  Serial.print("Autotune active?: ");
  if(Heater::autotune_on()){
    Serial.println("Yes");
  }else{
    Serial.println("No");
  }
}
