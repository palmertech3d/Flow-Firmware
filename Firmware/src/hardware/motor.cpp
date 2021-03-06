#include "hardware/motor.h"

AccelStepper Motor::m_extruder = AccelStepper(AccelStepper::DRIVER, PIN_M_EXTRUDER_STEP, PIN_M_EXTRUDER_DIR);
AccelStepper Motor::m_roller = AccelStepper(AccelStepper::DRIVER, PIN_M_ROLLER_STEP, PIN_M_ROLLER_DIR);
AccelStepper Motor::m_level = AccelStepper(AccelStepper::DRIVER, PIN_M_LEVEL_STEP, PIN_M_LEVEL_DIR);
AccelStepper Motor::m_winder = AccelStepper(AccelStepper::DRIVER, PIN_M_WINDER_STEP, PIN_M_WINDER_DIR);
long Motor::winder_bound_left = 23000;
long Motor::winder_bound_right = 14250;
bool Motor::level_homed = 0;
bool Motor::level_running = 0;

Tabular_t Motor::tabular_obj = Tabular_t();

void throwBadMotorNumber(){
  LOG_ERROR("BAD MOTOR NUMBER\n");
} // throwBadMotorNumber

void Motor::init() {
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
} // Motor::init

void Motor::start(int motor_num){
  switch (motor_num) {
  case EXTRUDER:
    // m_extruder.moveTo(-1000000);
    break;
  case PULLERS:
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

void Motor::stop(int motor_num) {
  Motor::setSpeed(motor_num, 0);
} // Motor::stop

float Motor::realUnitsToMotorTicks(int motor_num, float speed) {
  switch (motor_num) {
  case EXTRUDER:
    return M_EXTRUDER_STEPS_PER_MILLIRPM * speed;
    break;
  case PULLERS:
    return M_PULLER_STEPS_PER_MM * speed;
    break;
  case LEVEL:
    return M_LEVEL_STEPS_PER_MM * speed;
    break;
  case WINDER:
    return M_WINDER_STEPS_PER_MILLIRPM * speed;
    break;
  default:
    throwBadMotorNumber();
    return 0;
    break;
  } // switch
} // Motor::speedForMotor

float Motor::motorTicksToRealUnits(int motor_num, float tick_rate) {
  switch (motor_num) {
  case EXTRUDER:
    return tick_rate / M_EXTRUDER_STEPS_PER_MILLIRPM;
    break;
  case PULLERS:
    return tick_rate / M_PULLER_STEPS_PER_MM;
    break;
  case LEVEL:
    return tick_rate / M_LEVEL_STEPS_PER_MM;
    break;
  case WINDER:
    return tick_rate / M_WINDER_STEPS_PER_MILLIRPM;
    break;
  default:
    throwBadMotorNumber();
    return 0;
    break;
  } // switch
} // Motor::speedForMotor

void Motor::setSpeed(int motor_num, float motor_speed){
  static float speed_converted;
  speed_converted = realUnitsToMotorTicks(motor_num, motor_speed);

  switch (motor_num) {
  case EXTRUDER:
    if (speed_converted != 0 && global_blackboard.getFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP))
      break;
    #ifdef M_EXTRUDER_INVERT
    m_extruder.setSpeed(-speed_converted);
    #else // ifdef M_EXTRUDER_INVERT
    m_extruder.setSpeed(speed_converted);
    #endif // ifdef M_EXTRUDER_INVERT
    break;
  case PULLERS:
    #ifdef M_PULLER_INVERT
    m_roller.setSpeed(-speed_converted);
    #else // ifdef M_PULLER_INVERT
    m_roller.setSpeed(speed_converted);
    #endif // ifdef M_PULLER_INVERT
    break;
  case LEVEL:
    #ifdef M_LEVEL_INVERT
    m_level.setMaxSpeed(-speed_converted);
    m_level.setAcceleration(-speed_converted / 2);
    #else // ifdef M_LEVEL_INVERT
    m_level.setMaxSpeed(speed_converted);
    m_level.setAcceleration(speed_converted / 2);
    #endif // ifdef M_LEVEL_INVERT
    break;
  case WINDER:
    #ifdef M_WINDER_INVERT
    m_winder.setSpeed(-speed_converted);
    #else // ifdef M_WINDER_INVERT
    m_winder.setSpeed(speed_converted);
    #endif // ifdef M_WINDER_INVERT
    break;
  default:
    throwBadMotorNumber();
    break;
  } // switch

  // Triggering logging on motor changes (as opposed to polling)
  // means less data needs to clutter the serial output
  Motor::tabular_obj.triggerImmediateLogging();
} // Motor::setSpeed

float Motor::getSpeed(int motor_num){
  switch (motor_num) {
  case EXTRUDER:
    return motorTicksToRealUnits(EXTRUDER, m_extruder.speed());
  case PULLERS:
    return motorTicksToRealUnits(PULLERS, m_roller.speed());
  case LEVEL:
    return motorTicksToRealUnits(LEVEL, m_level.speed());
  case WINDER:
    return motorTicksToRealUnits(WINDER, m_winder.speed());
  default:
    throwBadMotorNumber();
    return -1;
  } // switch
} // Motor::getSpeed

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

void Motor::idle() {
  if (m_extruder.speed() != 0) {
    if (global_blackboard.getFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP)) {
      Motor::setSpeed(EXTRUDER, 0);
      LOG_WARN("Extruder stopping to prevent extrusion below min temp\n");
    }
  }
} // Motor::idle

void Motor::homeLevelWinder(){
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
} // Motor::homeLevelWinder

void Motor::enableReporter(uint16_t rate){
  Motor::tabular_obj.init(TDS_MOTOR, 4, Motor::motorTabularCallback,
                          F("extruder,puller,level,winder"));
  Motor::tabular_obj.setLoggingInterval(rate);
} // Motor::enableReporter

void Motor::disableReporter(){
  Motor::tabular_obj.stop();
} // Heater::disableReporter

TabularCallback_t Motor::motorTabularCallback(uint8_t col) {
  TabularCallback_t result;
  switch (col) {
  case 0:
    result.val.float_val = Motor::getSpeed(EXTRUDER);
    result.fmt = FMT_FLOAT;
    break;
  case 1:
    result.val.float_val = Motor::getSpeed(PULLERS);
    result.fmt = FMT_FLOAT;
    break;
  case 2:
    result.val.float_val = Motor::getSpeed(LEVEL);
    result.fmt = FMT_FLOAT;
    break;
  case 3:
    result.val.float_val = Motor::getSpeed(WINDER);
    result.fmt = FMT_FLOAT;
    break;
  } // switch
  // Get rid of negatives for easier data handling
  result.val.float_val = abs(result.val.float_val);
  return result;
} // Motor::motorTabularCallback

#ifdef UNIT_LEVEL_TESTING

TestResult_t Motor::TEST_preventColdExtrusion() {
  TestResult_t accumulator;
  TEST_ASSERT_EQUAL(Motor::m_extruder.speed(), 0, accumulator);
  Motor::m_extruder.setSpeed(10);
  TEST_ASSERT_EQUAL(Motor::m_extruder.speed(), 10, accumulator);
  global_blackboard.setFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP, 0);
  Motor::idle();
  TEST_ASSERT_EQUAL(Motor::m_extruder.speed(), 10, accumulator);
  global_blackboard.setFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP, 1);
  Motor::idle();
  TEST_ASSERT_EQUAL(Motor::m_extruder.speed(), 0, accumulator);
  global_blackboard.setFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP, 0);
  return accumulator;
} // Motor::TEST_preventColdExtrusion
#endif // ifdef UNIT_LEVEL_TESTING
