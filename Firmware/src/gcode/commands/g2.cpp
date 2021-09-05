#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

// g2 stops the given motor
void gcode::g2(int motor){
  Motor motorHandler;
  if (motor <= WINDER && motor >= EXTRUDER) {
    Serial.println(F("Stopping a motor..."));
    motorHandler.stop(motor);
  }
} // gcode::g2
