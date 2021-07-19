#include "../gcode.h"
#include "../../HAL/megaatmega2560/megaatmega2560.h"
#include "../../hardware/motor.h"

// g1 starts the given motor with the given speed

void gcode::g1(int motor, int speed){
  Motor motorHandler;
  if (motor == EXTRUDER || motor == ROLLERS || motor == WINDER){
    Serial.println(F("Starting a motor..."));
    motorHandler.set_speed(motor, speed);
    motorHandler.start(motor);
  }else if (motor == LEVEL){
    Serial.println(F("Starting the level winder..."));
    motorHandler.set_speed(motor, speed);
    motorHandler.start(motor);
  }else{
    Serial.println(F("Invalid motor number entered."));
  }
}
