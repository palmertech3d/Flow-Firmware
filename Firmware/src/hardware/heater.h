#ifndef HEATER_H
#define HEATER_H
#include "../HAL/megaatmega2560/megaatmega2560.h"
#include "../HAL/megaatmega2560/serial.h"
#include <PID_v1.h> // For PID control
#include <PID_AutoTune_v0.h>
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>
#include "config_defaults.h"
#include "logger.h"
#include "test/unit-testing.h"

namespace TR {
typedef enum TrStateType_enum {
  INACTIVE,
  RAMPUP,
  RAMPUP_OUT_OF_MIN_RAMP,
  AT_TEMP,
  AT_TEMP_OUT_OF_HYST,
  RUNAWAY
} TrStateType_t;

typedef struct TrConfig_struct {
  uint16_t ramp_up_max_watchdog_ms;
  uint16_t set_point_max_watchdog_ms;
  uint8_t set_point_hysteresis_deg_c;
  uint8_t set_min_rampup_deg_c_per_s;
  uint8_t min_temp;
  uint8_t max_temp;
} TrConfig_t;
typedef struct TrState_struct {
  int16_t rampup_last_temperature;
  uint32_t rampup_timer_ms;
  uint32_t watchdog_ms;
  TrStateType_t state_type;
} TrState_t;
} // namespace tr

class Heater {
public:

// Default constructor
Heater();

// Initializes the heater with a target temperature
static void init(double input_temp);

// Sets the temperature of the heater
static void set(double input_temp);

// Gets the current temperature of the heater
static double get();

// Gets the target temperature of the heater
static double get_target();

// Sets the PID constants for the heater
static void setConstants(double Kp_set, double Ki_set, double Kd_set);

// Gets the PID constants for the heater and stores them in a double* passed
// by reference
static void get_constants(double *constants_out);

// Experimental: Automatically tunes the PID constants for the heater
static void autotune_init();

// Returns true if autotune is active, false if not
static bool autotune_on();

// Updates the heater from the calculated PID value
// Must be called regularly
static void update();

// Kills heater from any heater errors
static void handleHeaterError(const __FlashStringHelper *error_name);

#ifdef UNIT_LEVEL_TESTING

static TestResult_t TEST_heater();
static TestResult_t TEST_runaway();
#endif // ifdef UNIT_LEVEL_TESTING

private:

// Run the PID autotune or store the values if it is done. Must be called in update() when auto_on is true.
static void runPidAutotuneUpdate();

// Run any necesary procedures for thermal runaway protection
static void stepThermalRunawayFsm(TR::TrConfig_t *config, TR::TrState_t *state, int16_t current_temperature, int16_t target);

static double temperature; // The current temperature of the hotend
static double target_temp; // The target temperature of the hotend
static double output; // The value calculated by the PID to write to the heater
static double Kp, Ki, Kd;
static MAX6675_Thermocouple thermometer;
static PID temp_controller;

// Autotune
static PID_ATune pid_auto;
static bool auto_on;

// Thermal runaway
static TR::TrConfig_t tr_config;
static TR::TrState_t tr_state;
}

;

#endif // HEATER_H
