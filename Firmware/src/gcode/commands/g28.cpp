// G28 homes the level winder
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../gcode.h"
#include <AccelStepper.h>
#include "../../hardware/motor.h"

// main G28 function: call this to execute G28
// Homes the level winder; this function will delay all future action
// until the winder has been homed
void gcode::g28(){
  Serial.println(F("Homing the level winder..."));
  Motor motorHandler;
  motorHandler.home_level_winder();

}
