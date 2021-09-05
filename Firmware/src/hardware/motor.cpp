#include "motor.h"

AccelStepper Motor::m_extruder = AccelStepper(AccelStepper::DRIVER, PIN_M_EXTRUDER_STEP, PIN_M_EXTRUDER_DIR);
AccelStepper Motor::m_roller = AccelStepper(AccelStepper::DRIVER, PIN_M_ROLLER_STEP, PIN_M_ROLLER_DIR);
AccelStepper Motor::m_level = AccelStepper(AccelStepper::DRIVER, PIN_M_LEVEL_STEP, PIN_M_LEVEL_DIR);
AccelStepper Motor::m_winder = AccelStepper(AccelStepper::DRIVER, PIN_M_WINDER_STEP, PIN_M_WINDER_DIR);
long Motor::winder_bound_left = 23000;
long Motor::winder_bound_right = 14250;
bool Motor::level_homed = 0;
bool Motor::level_running = 0;

Motor::Motor(){
  m_extruder.setMaxSpeed(300);
  m_extruder.setAcceleration(100.0);
  // m_extruder.setSpeed(100.0);

  m_roller.setMaxSpeed(1000);
  m_roller.setAcceleration(100.0);
  // m_roller.setSpeed(100.0);

  // m_level.setMaxSpeed(10000);
  // m_level.setAcceleration(5000);
  // m_level.setSpeed(10000);

  m_winder.setMaxSpeed(1000);
  m_winder.setAcceleration(100.0);
  // m_winder.setSpeed(100.0);
}

void Motor::start(int motor_num){
  switch (motor_num) {
  case EXTRUDER:
    // m_extruder.moveTo(-1000000);
    break;
  case ROLLERS:
    // m_roller.moveTo(1000000);
    break;
  case LEVEL:
    level_running = true;
    m_level.moveTo(winder_bound_right);
    break;
  case WINDER:
    // m_winder.moveTo(1000000);
    break;
  } // switch
} // Motor::start

void Motor::stop(int motor_num){
  switch (motor_num) {
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
  } // switch
} // Motor::stop

void Motor::set_speed(int motor_num, float motor_speed){
  switch (motor_num) {
  case EXTRUDER:
    m_extruder.setSpeed(-1 * motor_speed);
    break;
  case ROLLERS:
    m_roller.setSpeed(-1 * motor_speed);
    break;
  case LEVEL:
    m_level.setMaxSpeed(motor_speed);
    m_level.setAcceleration(motor_speed / 2);
    break;
  case WINDER:
    m_winder.setSpeed(motor_speed);
    break;
  } // switch
} // Motor::set_speed

float Motor::get_speed(int motor_num){
  switch (motor_num) {
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
  } // switch
} // Motor::get_speed

void Motor::run(){
  m_extruder.runSpeed();
  m_roller.runSpeed();
  m_winder.runSpeed();
  m_level.run();

  if (level_homed && level_running) {
    if (m_level.currentPosition() >= winder_bound_left) {
      m_level.moveTo(winder_bound_right);
    } else if (m_level.currentPosition() <= winder_bound_right) {
      m_level.moveTo(winder_bound_left);
    }
  }
} // Motor::run

void Motor::home_level_winder(){
  level_homed = 0;

  // Get level winder ready to be homed
  m_level.setCurrentPosition(0);
  m_level.setMaxSpeed(10000);
  // m_level.setSpeed(10000);
  m_level.setAcceleration(5000);
  m_level.moveTo(-100000);

  // TODO: move this to Motor::run()
  while (!level_homed) {
    if (digitalRead(PIN_LIM_SWITCH_WINDER) == LOW) {
      level_homed = 1; // Let us know that the level has been homed
      m_level.setCurrentPosition(0); // Set our starting pos to 0
      m_level.moveTo(19500); // Move the level winder out from the limit switch
    }
  }
} // Motor::home_level_winder

void Motor::set_winder_bounds(int left, int right){
  winder_bound_left = left;
  winder_bound_right = right;
} // Motor::set_winder_bounds
