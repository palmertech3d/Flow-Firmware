// G28 homes the level winder
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../gcode.h"
#include <AccelStepper.h>

// main G28 function: call this to execute G28
// Homes the level winder; this function will delay all future action
// until the winder has been homed
void gcode::g28(){
  Serial.println(F("Homing the level winder..."));

  bool levelHomed = 0;
  AccelStepper m_level(1, M_LEVEL_STEP, M_LEVEL_DIR);
  // Get level winder ready to be homed
  m_level.setCurrentPosition(0);
  m_level.moveTo(-100000); // Get the level winder ready to move fast towards the limit switch
  m_level.setMaxSpeed(5000);
  m_level.setSpeed(5000);
  m_level.setAcceleration(2000);

  while(!levelHomed){
    m_level.run(); // Move the level winder towards the limit switch

    if(digitalRead(WIND_LIM_SWITCH) == LOW){
      levelHomed = 1; // Let us know that the level has been homed
      m_level.setCurrentPosition(0); // Set our starting pos to 0
      m_level.moveTo(10000); // Move the level winder out from the limit switch

    }
  }

  while(m_level.currentPosition() != 10000){
    m_level.run();
    }
}
