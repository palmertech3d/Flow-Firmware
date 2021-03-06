#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include <Arduino.h>
#include "../../hardware/heater.h"
#include "../../hardware/motor.h"
#include <MemoryFree.h>

// M503 reports settings to the terminal
void GcodeExecuter::m503(GcodeCommand_t cmd){
  Serial.println(F("Current settings for Flow Extruder:"));
  Serial.print(F("Remaining RAM: ")); Serial.println(freeMemory());
  Serial.print(F("Current hotend temp: ")); Serial.println((int)Heater::get());
  Serial.print(F("Target hotend temp: ")); Serial.println((int)Heater::get_target());
  double constants[3];
  Heater::get_constants(constants);
  Serial.println(F("Hotend PID constants: "));
  Serial.print(F("\tKp: ")); Serial.println(constants[0]);
  Serial.print(F("\tKi: ")); Serial.println(constants[1]);
  Serial.print(F("\tKd: ")); Serial.println(constants[2]);
  Serial.print(F("Autotune active?: "));
  if (Heater::isAutotuneOn()) {
    Serial.println(F("Yes"));
  } else {
    Serial.println(F("No"));
  }
  Serial.println(F("Motor speeds (steps/sec):"));
  Serial.print(F("Extruder: "));
  Serial.println(Motor::getSpeed(EXTRUDER));
  Serial.print(F("Rollers: "));
  Serial.println(Motor::getSpeed(PULLERS));
  Serial.print(F("Level Winder: "));
  Serial.println(Motor::getSpeed(LEVEL));
  Serial.print(F("Winder: "));
  Serial.println(Motor::getSpeed(WINDER));
} // GcodeExecuter::m503
