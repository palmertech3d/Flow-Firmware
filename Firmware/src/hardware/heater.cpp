#include "heater.h"
#include "config_defaults.h"
#include "logger.h"
#include <stdlib.h>
#include <Arduino.h>

// Initialize static variables for Heater class
double Heater::temperature = 0;
double Heater::target_temp = 0;
double Heater::output = 0;
double Heater::Kp = PID_HTR0_P, Heater::Ki = PID_HTR0_I, Heater::Kd = PID_HTR0_D; // PID constants, default vals are samples
MAX6675_Thermocouple Heater::thermometer = MAX6675_Thermocouple(PIN_THERMO_SCK, PIN_THERMO_CS, PIN_THERMO_SO);
PID Heater::temp_controller = PID(&Heater::temperature, &Heater::output, &Heater::target_temp, Heater::Kp, Heater::Ki, Heater::Kd, DIRECT);
bool Heater::autotune_on = false;
atune::GenericAutotune_t Heater::atune_handler = atune::GenericAutotune_t();
Tabular_t Heater::tabular_obj = Tabular_t();
TabularData_t Heater::tabular_ptr_arr[5] = {
  {.ptr = {.double_ptr = &Heater::temperature}, .fmt = FMT_DOUBLE},
  {.ptr = {.double_ptr = &Heater::target_temp}, .fmt = FMT_DOUBLE},
  {.ptr = {.double_ptr = &Heater::output}, .fmt = FMT_DOUBLE},
  {.ptr = {.uint8_t_ptr = ((uint8_t *)&Heater::autotune_on)}, .fmt = FMT_UINT8_T},
  {.ptr = {.uint8_t_ptr = ((uint8_t *)&Heater::tr_state)}, .fmt = FMT_UINT8_T}
};

TR::TrConfig_t Heater::tr_config;
TR::TrState_t Heater::tr_state;

// Default constructor
Heater::Heater(){
}

// Initializes the heater with a target temperature
void Heater::init(){
  Heater::set(0);
  temperature = thermometer.readCelsius();
  temp_controller.SetMode(AUTOMATIC);

  // Set up thermal runaway constants
  tr_config.ramp_up_max_watchdog_ms = HTR0_TR_MAX_TIME_OUT_OF_MIN_RAMP_MS;
  tr_config.set_point_max_watchdog_ms = HTR0_TR_MAX_TIME_OUT_OF_HYST_MS;
  tr_config.set_point_hysteresis_deg_c = HTR0_TR_TEMP_HYSTERESIS_C;
  tr_config.set_min_rampup_deg_c_per_sample = HTR0_TR_MIN_RAMPUP_DEGC_PER_SAMPLE;
  tr_config.rampup_sample_period_ms = HTR0_TR_RAMPUP_SAMPLE_DELAY_MS;
  tr_config.min_temp = HTR0_MIN_TEMP;
  tr_config.max_temp = HTR0_MAX_TEMP;
} // Heater::init

// Sets the temperature of the heater
void Heater::set(double input_temp){
  target_temp = input_temp;
} // Heater::set

// Gets the current temperature of the heater
double Heater::get(){
  return temperature;
} // Heater::get

// Gets the target temperature of the heater
double Heater::get_target(){
  return target_temp;
} // Heater::get_target

// Sets the PID constants for the heater
void Heater::setConstants(double Kp_set, double Ki_set, double Kd_set){
  Kp = Kp_set;
  Ki = Ki_set;
  Kd = Kd_set;
  temp_controller.SetTunings(Kp, Ki, Kd);
} // Heater::setConstants

void Heater::get_constants(double *constants_out){
  constants_out[0] = Kp;
  constants_out[1] = Ki;
  constants_out[2] = Kd;
} // Heater::get_constants

void Heater::enableReporter(uint16_t rate){
  Heater::tabular_obj.init(TDS_HEATER,
                           sizeof(Heater::tabular_ptr_arr) / sizeof(Heater::tabular_ptr_arr[0]), Heater::tabular_ptr_arr,
                           F("temperature,target_temp,output,autotune_on,tr_state"));
  Heater::tabular_obj.setLoggingInterval(rate);
} // Heater::enableReporter

void Heater::disableReporter(){
  Heater::tabular_obj.stop();
} // Heater::disableReporter

void Heater::enableAtuneReporter(uint16_t rate){
  atune_handler.beginTabularReporting(TDS_TEMP_ATUNE, rate);
} // Heater::enableAtuneReporter

void Heater::disableAtuneReporter(){
  atune_handler.endTabularReporting();
} // Heater::disableAtuneReporter

void Heater::autotune_init(uint16_t target_temp, uint16_t band_distance){
  autotune_on = true;
  atune::AutotuneConfig_t cfg;
  cfg.center_value = target_temp;
  cfg.hysteresis_distance = band_distance;
  cfg.output_min_allowable = 0;
  cfg.output_max_allowable = 255;
  cfg.output_ptr = &output;
  cfg.centerline_allowed_error = 0.01;
  cfg.duty_allowed_error = 0.1;
  atune_handler.init(cfg);
} // Heater::autotune_init

void Heater::autotuneStop(){
  autotune_on = false;
  atune_handler.stop();
} // Heater::autotune_init

bool Heater::isAutotuneOn(){
  return autotune_on;
} // Heater::autotune_on

void Heater::runPidAutotuneUpdate() {
  if (!autotune_on) {
    LOG_ERROR("Autotune runtime attempted without autotune being enabled");
    return;
  }
  atune_handler.runtime(temperature);
} // Heater::runPidAutotuneUpdate

void Heater::handleHeaterError(const __FlashStringHelper *error_name) {
  LOG_WARN("HEATER ERROR THROWN\n");
  LOG_INFO(F("Error desciption: ")); LOG_INFO(error_name); LOG_INFO(F("\n"));
  Heater::set(0);
  autotune_on = false;
  autotuneStop();
} // Heater::handleHeaterError

void Heater::stepThermalRunawayFsm(TR::TrConfig_t *config, TR::TrState_t *state, int16_t current_temperature, int16_t target) {
  #define _TEMP_INSIDE_HYSTERESIS() (current_temperature >= target - config->set_point_hysteresis_deg_c && current_temperature <= target + config->set_point_hysteresis_deg_c)
  #define _TEMP_INSIDE_RAMPUP() (current_temperature - state->rampup_last_temperature >= config->set_min_rampup_deg_c_per_sample)

  if (target == 0) {
    state->rampup_last_temperature = 0;
    state->state_type = TR::INACTIVE;
  } else if (current_temperature > config->max_temp || current_temperature < config->min_temp) {
    // Does not check min/max if temps aren't set, since it makes testing w/o heater easier
    state->state_type = TR::RUNAWAY;
  }
  switch (state->state_type) {
  case TR::INACTIVE: // Nothing happening
    if (target > 0) {
      state->state_type = TR::RAMPUP;
      state->rampup_last_temperature = current_temperature;
      state->rampup_timer_ms = millis() + config->rampup_sample_period_ms;
    }
    break;
  case TR::RAMPUP: // Heater is powered and heating up initially
    // Check rampup temperature periodically
    if (state->rampup_timer_ms <= millis()) {
      if (!_TEMP_INSIDE_RAMPUP()) {
        state->watchdog_ms = millis(); // Start watchdog
        state->state_type = TR::RAMPUP_OUT_OF_MIN_RAMP;
      }
      // Update values
      state->rampup_last_temperature = current_temperature;
      state->rampup_timer_ms = millis() + config->rampup_sample_period_ms;
    }
    if (_TEMP_INSIDE_HYSTERESIS()) {
      state->state_type = TR::AT_TEMP;
    }
    break;
  case TR::RAMPUP_OUT_OF_MIN_RAMP: // Heater is ramping up, but the minumum ramp rate is not reached
    if (state->rampup_timer_ms <= millis()) {
      if (_TEMP_INSIDE_RAMPUP()) {
        // Go back to regular rampup
        state->state_type = TR::RAMPUP;
      }
      // Update values
      state->rampup_last_temperature = current_temperature;
      state->rampup_timer_ms = millis() + config->rampup_sample_period_ms;
    }
    if (millis() - state->watchdog_ms >= config->ramp_up_max_watchdog_ms) {
      state->state_type = TR::RUNAWAY;
    }
    break;
  case TR::AT_TEMP: // Heater is powered and maintaining its target temperature
    if (!_TEMP_INSIDE_HYSTERESIS()) {
      state->watchdog_ms = millis(); // Start watchdog
      state->state_type = TR::AT_TEMP_OUT_OF_HYST;
    }
    break;
  case TR::AT_TEMP_OUT_OF_HYST: // Heater is powered and maintaining its target temperature, but out of the hysteresis
    if (_TEMP_INSIDE_HYSTERESIS()) {
      // Back to safe zone
      state->state_type = TR::AT_TEMP;
    } else if (millis() - state->watchdog_ms >= config->set_point_max_watchdog_ms) {
      state->state_type = TR::RUNAWAY;
    }
    break;
  case TR::RUNAWAY:
    Heater::handleHeaterError(F("TERMAL RUNAWAY"));
    break;
  } // switch
} // Heater::stepThermalRunawayFsm

void Heater::updateFlagsFromTemperature(int16_t temperature) {
  if (temperature < MIN_EXTRUSION_TEMP_C) {
    global_blackboard.setFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP, 1);
  } else {
    global_blackboard.setFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP, 0);
  }
} // updateFlagsFromTemperature

static uint32_t next_check_time = 0;

void Heater::update() {
  double new_temperature;
  if (millis() >= next_check_time) {
    // Must poll sensor slowly; unclear if this is a library or hardware issue.
    new_temperature = thermometer.readCelsius();
    next_check_time = millis() + 500;
  }
  if (new_temperature == 0) { // Sensor was not polled
    new_temperature = temperature;
  }
  // if ((int)new_temperature != (int)temperature) {
  //   // Prints out temperature changes
  //   Serial.print(F("Hotend: ")); Serial.print((int)temperature); Serial.println(F("C"));
  //   LOG_DEBUG(F("Output: ")); LOG_DEBUG(output); LOG_DEBUG('\n');
  //   LOG_DEBUG(F("Direct: ")); LOG_DEBUG(thermometer.readCelsius()); LOG_DEBUG('\n');
  // }

  updateFlagsFromTemperature(new_temperature);

  temperature = new_temperature;

  stepThermalRunawayFsm(&tr_config, &tr_state, (int16_t)temperature, (int16_t) target_temp);

  if (autotune_on) { // Autotune uses bang-bang control so just skip the PID controller
    runPidAutotuneUpdate();
    return;
  }

  temp_controller.Compute(); // Compute & store control values into `output`

  // TODO: Convert this to a hardware abstraction where the output is synced to
  // zero-crossings of the AC input
  analogWrite(PIN_HEATER, output);
} // Heater::update

// TEST FUNCTIONS /////////////////////////////////////////////////////////////

#ifdef UNIT_LEVEL_TESTING

TestResult_t Heater::TEST_heater() {
  TestResult_t accumulator;
  Heater::set(25);
  TEST_ASSERT_EQUAL(int16_t(Heater::get_target()), 25, accumulator);
  Heater::set(0);
  TEST_ASSERT_EQUAL(int16_t(Heater::get_target()), 0, accumulator);
  updateFlagsFromTemperature(MIN_EXTRUSION_TEMP_C - 1);
  Heater::update();
  TEST_ASSERT_EQUAL(global_blackboard.getFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP), true, accumulator);
  updateFlagsFromTemperature(MIN_EXTRUSION_TEMP_C + 1);
  TEST_ASSERT_EQUAL(global_blackboard.getFlag(BBFLAG_BELOW_EXTRUSION_MINTEMP), false, accumulator);
  updateFlagsFromTemperature(0);

  Heater::setConstants(1, 2, 3);
  TEST_ASSERT_EQUAL(int16_t(Heater::Kp), 1, accumulator);
  TEST_ASSERT_EQUAL(int16_t(Heater::Ki), 2, accumulator);
  TEST_ASSERT_EQUAL(int16_t(Heater::Kd), 3, accumulator);
  return accumulator;
} // Heater::TEST_heater

TestResult_t Heater::TEST_runaway() {
  TestResult_t accumulator;

  TR::TrConfig_t config;
  TR::TrState_t state;

  // Test values to be validated
  #define HYST 3 // Used later for off-by-one testing
  config.ramp_up_max_watchdog_ms = 500;
  config.set_point_max_watchdog_ms = 200; // Must be 100ms apart from above WD
  config.set_point_hysteresis_deg_c = HYST;
  config.set_min_rampup_deg_c_per_sample = 10;
  config.rampup_sample_period_ms = 10;
  config.min_temp = 0;
  config.max_temp = 200;

  // Confirm exit and re-entry of INACTIVE state
  state.state_type = TR::INACTIVE;
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Does not change from inactive
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Does not change from inactive (again)
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP, accumulator); // Changes to rampup
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP, accumulator); // Does not exit rampup
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Exits rampup when target=0

  // Confirm entry and exiting of RAMPUP
  state.state_type = TR::INACTIVE;
  uint32_t last_rampup_time = 0;
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Does not change from inactive
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP, accumulator); // Changes to rampup
  last_rampup_time = state.rampup_timer_ms;
  while (state.rampup_timer_ms > millis()); // Wait for timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 25, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP, accumulator); // Stays in rampup when temp ramps up correctly
  TEST_ASSERT_NEQUAL(last_rampup_time, state.rampup_timer_ms, accumulator); // Timer was reset
  last_rampup_time = state.rampup_timer_ms;
  while (state.rampup_timer_ms > millis()); // Wait for timeout again
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 30, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP_OUT_OF_MIN_RAMP, accumulator); // Exits rampup when temp ramps up too slowly
  TEST_ASSERT_NEQUAL(last_rampup_time, state.rampup_timer_ms, accumulator); // Timer was reset
  delay(config.ramp_up_max_watchdog_ms - 100); // Come close to watchdog timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 40, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP, accumulator); // Re-entes rampup when temp is fine again
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Exits rampup when target=0

  // Confirm entry and exiting of AT_TEMP
  state.state_type = TR::INACTIVE;
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100); // Skip inactive and rampup tests
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Enters AT_TEMP when it has reached hysteresis
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Maintains AT_TEMP
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 + HYST, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Maintains AT_TEMP
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 + HYST + 1, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP_OUT_OF_HYST, accumulator); // Notices leaving hysteresis
  delay(config.set_point_max_watchdog_ms - 100); // Come close to watchdog timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 + HYST + 15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP_OUT_OF_HYST, accumulator); // Maintains state
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 + HYST, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Goes back to normal
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 1, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP_OUT_OF_HYST, accumulator); // Notices leaving hysteresis (negative)
  delay(config.set_point_max_watchdog_ms - 100); // Come close to watchdog timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP_OUT_OF_HYST, accumulator); // Maintains state
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Goes back to normal
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Exits rampup when target=0

  // Confirm rampup watchdog
  state.state_type = TR::INACTIVE;
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP, accumulator); // Changes to rampup
  while (state.rampup_timer_ms > millis()); // Wait for timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 20, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RAMPUP_OUT_OF_MIN_RAMP, accumulator); // Exits rampup
  delay(config.ramp_up_max_watchdog_ms + 1); // Wait for watchdog timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 25, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RUNAWAY, accumulator); // Enters runaway after watchdog time passes
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Exits rampup when target=0

  // Confirm AT_TEMP watchdog
  state.state_type = TR::INACTIVE;
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Exits rampup
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 1, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP_OUT_OF_HYST, accumulator); // Exits AT_TEMP
  delay(config.ramp_up_max_watchdog_ms + 1); // Wait for watchdog timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 1, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RUNAWAY, accumulator); // Enters runaway after watchdog time passes
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Exits rampup when target=0

  // Confirm min/max errors
  state.state_type = TR::INACTIVE;
  LOG_INFO(F("Running thermal runaway test; RUNAWAY ERRORS ARE NORMAL AND EXPECTED.\n"));
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ -15, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RUNAWAY, accumulator); // minmium temp runaway
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 0, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 200, /* target temp*/ 100);
  TEST_ASSERT_NEQUAL(state.state_type, TR::RUNAWAY, accumulator);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 220, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RUNAWAY, accumulator); // maximum temp runaway
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 0, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator);

  // Confirm RUNAWAY disables heater
  Heater::set(25);
  TEST_ASSERT_EQUAL(int16_t(Heater::get_target()), 25, accumulator); // Target set to 0
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 100);
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::AT_TEMP, accumulator); // Sanity check state
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 1, /* target temp*/ 100);
  delay(config.ramp_up_max_watchdog_ms + 1); // Wait for watchdog timeout
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 1, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(state.state_type, TR::RUNAWAY, accumulator); // Sanity check state again
  LOG_INFO(F("Running thermal runaway test; RUNAWAY ERRORS ARE NORMAL AND EXPECTED.\n"));
  // Allow RUNAWAY state actions to happen
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 100 - HYST - 1, /* target temp*/ 100);
  TEST_ASSERT_EQUAL(Heater::get_target(), 0, accumulator); // Target set to 0
  stepThermalRunawayFsm(&config, &state, /* current temp*/ 15, /* target temp*/ 0);
  TEST_ASSERT_EQUAL(state.state_type, TR::INACTIVE, accumulator); // Exits when target=0

  return accumulator;
} // TEST_gcode
#endif // ifdef UNIT_LEVEL_TESTING
