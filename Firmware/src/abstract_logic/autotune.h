/**
 * @Author: Nick Steele
 * @Date:   19:22 Oct 07 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 8:25 Oct 08 2021
 */

// Uses the Relay Method to autotune the heater within a given range around a given setpoint.
// https://en.wikipedia.org/wiki/PID_controller#Relay_(%C3%85str%C3%B6m%E2%80%93H%C3%A4gglund)_method

#ifndef GENERIC_AUTOTUNE_H
#define GENERIC_AUTOTUNE_H

#include <Arduino.h>
#include <stdint.h>
#include "logger.h"
#include "test/unit-testing.h"
#include "tabular/tabular.h"

#define ATUNE_CYCLES_REQUIRED_AT_STARTUP 2
#define ATUNE_CYCLES_REQUIRED_FOR_MEASUREMENT 8

namespace atune {
typedef enum TempAtuneState_enum {
  STOPPED, // Not running, ready to start.
  BEGIN_ATUNE, // Prepare values for running
  RAMPUP, // Initial heating
  BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH, // Adjust output high/low values to get a 50% duty cycle.
  BANGBANG_DUTY_COMPENSATION_OUTPUT_LOW, // Same as above. This is the off time of the high/low cycling
  OSCILLATE_WHILE_READING, // With the 50% duty cycle, begin oscillating and run until the period and amplitude stablize to within a certain percent.
  COMPUTE_AND_STORE_DATA // Compute and store the values found from the oscillation.
} TempAtuneState_t;
typedef struct TempAutotune_struct {
  double center_value;
  double hysteresis_distance;
  double output_min_allowable;
  double output_max_allowable;
  double *output_ptr;
  double centerline_allowed_error; // pct of 1; This is the accuracy to use for floating point comparisons
  double duty_allowed_error; // pct of 1; This is the accuracy to use for floating point comparisons
  double compensation_rate; // How fast it will try to decend upon a 50% duty cycle
} AutotuneConfig_t;

class GenericAutotune_t {
private:
AutotuneConfig_t cfg;
TempAtuneState_t state;
uint32_t last_time_increased_past_middle;
uint32_t last_time_decreased_past_middle;
float latest_duty_cycle;
// float previous_cycle_duty_cycle;
float output_min_compensated, output_max_compensated;
uint8_t flags;
int16_t cycles_completed;

// Used in computing amplitude and frequency of oscillations
float min_measured_in_cycle;
float max_measured_in_cycle;
uint32_t average_oscillation_period;
float average_oscillation_min;
float average_oscillation_max;

// PID values assumed
float computed_p, computed_i, computed_d;

// For reporting data while autotuning; must be enabled with a tabular ID
TabularData_t tabular_ptr_arr[16] = {
  {.ptr = {.float_ptr = &computed_p}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &computed_i}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &computed_d}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &latest_duty_cycle}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &output_min_compensated}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &output_max_compensated}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &min_measured_in_cycle}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &max_measured_in_cycle}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &average_oscillation_min}, .fmt = FMT_FLOAT},
  {.ptr = {.float_ptr = &average_oscillation_max}, .fmt = FMT_FLOAT},
  {.ptr = {.double_ptr = cfg.output_ptr}, .fmt = FMT_DOUBLE},

  {.ptr = {.uint32_t_ptr = &last_time_increased_past_middle}, .fmt = FMT_UINT32_T},
  {.ptr = {.uint32_t_ptr = &last_time_decreased_past_middle}, .fmt = FMT_UINT32_T},
  {.ptr = {.uint32_t_ptr = &average_oscillation_period}, .fmt = FMT_UINT32_T},
  {.ptr = {.int16_t_ptr = &cycles_completed}, .fmt = FMT_INT16_T},
  {.ptr = {.uint8_t_ptr = ((uint8_t *)&state)}, .fmt = FMT_UINT8_T}
};

Tabular_t tabular_obj;

/**
 * Computes and stores the duty cycle, then compensates for duty cycles outside
 * of the allowed error
 *
 * @return TRUE:  The min/max values were compensated, need more cycles
 *         FALSE: Compensation copmlete
 */
bool computeDutyCycleAndCompensate();

void computePidValsFromAverages();

public:

bool getPid(double *p, double *i, double *d);

bool runtime(double last_measurement);

/**
 * Start reporting the data using Tabular.
 *
 * @param src_id The tabular ID for the object's PID autotune data. Do NOT use
 *               the object's ID here, since it'll jumble all the data.
 */
void beginTabularReporting(TabularSource_t src_id, uint16_t interval);

void endTabularReporting();

void init(AutotuneConfig_t cfg);

void stop();

#ifdef UNIT_LEVEL_TESTING

/**
 * Megafunction which loosely simulates a system and controller in need of PID tuning.
 */
TestResult_t TEST_autotune();
#endif // ifdef UNIT_LEVEL_TESTING
} // class GenericAutotune_t

;
} // namespace atune

#endif /* end of include guard: GENERIC_AUTOTUNE_H */
