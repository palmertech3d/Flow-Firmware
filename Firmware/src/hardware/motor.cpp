#include "motor.h"

AccelStepper Motor::m_extruder = AccelStepper(AccelStepper::DRIVER, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper Motor::m_roller = AccelStepper(AccelStepper::DRIVER, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper Motor::m_level = AccelStepper(AccelStepper::DRIVER, M_LEVEL_STEP, M_LEVEL_DIR);
AccelStepper Motor::m_winder = AccelStepper(AccelStepper::DRIVER, M_WINDER_STEP, M_WINDER_DIR);

Motor::Motor(){
  m_extruder.setMaxSpeed(150.0);
  m_extruder.setAcceleration(100.0);
  //m_extruder.setSpeed(100.0);

  m_roller.setMaxSpeed(1000);
  m_roller.setAcceleration(100.0);
  //m_roller.setSpeed(100.0);

  //m_level.setMaxSpeed(200.0);
  //m_level.setAcceleration(100.0);
  //m_level.setSpeed(100.0);

  m_winder.setMaxSpeed(100);
  m_winder.setAcceleration(100.0);
  //m_winder.setSpeed(100.0);

}

void Motor::start(int motor_num){
  switch (motor_num){
    case EXTRUDER:
      //m_extruder.moveTo(-1000000);
      break;
    case ROLLERS:
      //m_roller.moveTo(1000000);
      break;
    case LEVEL:
      // run the level winder
      break;
    case WINDER:
      //m_winder.moveTo(1000000);
      break;
  }

}

void Motor::stop(int motor_num){
  switch(motor_num){
    case EXTRUDER:
      m_extruder.setSpeed(0);
      break;
    case ROLLERS:
      m_roller.setSpeed(0);
      break;
    case LEVEL:
      m_level.stop();
      break;
    case WINDER:
      m_winder.setSpeed(0);
      break;
  }
}

void Motor::set_speed(int motor_num, float motor_speed){
  switch (motor_num){
    case EXTRUDER:
      m_extruder.setSpeed(-1*motor_speed);
      break;
    case ROLLERS:
      m_roller.setSpeed(-1*motor_speed);
      break;
    case LEVEL:
      m_level.setMaxSpeed(motor_speed);
      break;
    case WINDER:
      m_winder.setSpeed(motor_speed);
      break;
  }
}

float Motor::get_speed(int motor_num){
  switch (motor_num){
    case EXTRUDER:
      return m_extruder.speed();
    case ROLLERS:
      return m_roller.speed();
    case LEVEL:
      return m_level.speed();
    case WINDER:
      return m_winder.speed();
    default:
      return -1;
  }
}

void Motor::run(){
  m_extruder.runSpeed();
  m_roller.runSpeed();
  m_level.run();
  m_winder.runSpeed();
}

void Motor::home_level_winder(){
  bool levelHomed = 0;

  // Get level winder ready to be homed
  m_level.setCurrentPosition(0);
  m_level.moveTo(-100000);
  m_level.setMaxSpeed(10000);
  m_level.setSpeed(10000);
  m_level.setAcceleration(5000);

  while(!levelHomed){
    if(digitalRead(WIND_LIM_SWITCH) == LOW){
      levelHomed = 1; // Let us know that the level has been homed
      m_level.setCurrentPosition(0); // Set our starting pos to 0
      m_level.moveTo(19500); // Move the level winder out from the limit switch

    }
  }
}
