// G28 homes the level winder
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../gcode.h"
#include <AccelStepper.h>
#include "../../hardware/motor.h"

void GcodeExecuter::g28(GcodeCommand_t cmd) {
  bool home_winder;
  bool home_all = gcodeNoArgsGiven(cmd);
  if (!home_all) {
    home_winder = gcodeArgWasGivenFor('w', cmd);
  }
  if (home_winder || home_all) {
    Serial.println(F("Homing the level winder..."));
    Motor motor_handler;
    motor_handler.home_level_winder();
  } else {
    Serial.println(F("No valid axes chosen to home."));
  }
} // GcodeExecuter::g28
