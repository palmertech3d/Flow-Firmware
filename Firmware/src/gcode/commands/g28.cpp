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
    LOG_INFO(F("Homing the level winder...\n"));
    Motor::homeLevelWinder();
  } else {
    LOG_WARN("No valid axes chosen to home.\n");
  }
} // GcodeExecuter::g28
