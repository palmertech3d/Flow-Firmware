#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>
#include "../../hardware/heater.h"

// M503 reports settings to the terminal
void gcode::m503(){
  Serial.println(F("Current settings for Flow Extruder:"));
  Serial.print(F("Current hotend temp: ")); Serial.println((int)Heater::get());
  Serial.print(F("Target hotend temp: ")); Serial.println((int)Heater::get_target());
  double constants[3];
  Heater::get_constants(constants);
  Serial.println(F("Hotend PID constants: "));
  Serial.print(F("\tKp: ")); Serial.println(constants[0]);
  Serial.print(F("\tKi: ")); Serial.println(constants[1]);
  Serial.print(F("\tKd: ")); Serial.println(constants[2]);
  Serial.print(F("Autotune active?: "));
  if(Heater::autotune_on()){
    Serial.println(F("Yes"));
  }else{
    Serial.println(F("No"));
  }
}
