#include "motor.h"

AccelStepper Motor::m_extruder = AccelStepper(AccelStepper::DRIVER, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper Motor::m_roller = AccelStepper(AccelStepper::DRIVER, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper Motor::m_level = AccelStepper(AccelStepper::DRIVER, M_LEVEL_STEP, M_LEVEL_DIR);
AccelStepper Motor::m_winder = AccelStepper(AccelStepper::DRIVER, M_WINDER_STEP, M_WINDER_DIR);

Motor::Motor(){
  m_extruder.setMaxSpeed(150.0);
  m_extruder.setAcceleration(100.0);
  //m_extruder.setSpeed(100.0);

  m_roller.setMaxSpeed(25);
  m_roller.setAcceleration(100.0);
  //m_roller.setSpeed(100.0);

  m_level.setMaxSpeed(200.0);
  m_level.setAcceleration(100.0);
  //m_level.setSpeed(100.0);

  m_winder.setMaxSpeed(25);
  m_winder.setAcceleration(100.0);
  //m_winder.setSpeed(100.0);

}

void Motor::start(int motor_num){
  switch (motor_num){
    case EXTRUDER:
      m_extruder.moveTo(-1000000);
      break;
    case ROLLERS:
      m_roller.moveTo(1000000);
      break;
    case LEVEL:
      // run the level winder
      //m_level.moveTo(1000000);
      break;
    case WINDER:
      m_winder.moveTo(1000000);
      break;
  }

}

void Motor::stop(int motor_num){
  switch(motor_num){
    case EXTRUDER:
      m_extruder.stop();
      break;
    case ROLLERS:
      m_roller.stop();
      break;
    case LEVEL:
      m_level.stop();
      break;
    case WINDER:
      m_winder.stop();
      break;
  }
}

void Motor::set_speed(int motor_num, float motor_speed){
  switch (motor_num){
    case EXTRUDER:
      m_extruder.setMaxSpeed(motor_speed);
      break;
    case ROLLERS:
      m_roller.setMaxSpeed(motor_speed);
      break;
    case LEVEL:
      m_level.setMaxSpeed(motor_speed);
      break;
    case WINDER:
      m_winder.setMaxSpeed(motor_speed);
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
  m_extruder.run();
  m_roller.run();
  m_level.run();
  m_winder.run();
}
