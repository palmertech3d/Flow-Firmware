/**
 * @Author: Nick Steele
 * @Date:   19:23 Oct 07 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 8:26 Oct 08 2021
 */

#include <float.h>
#include "abstract_logic/autotune.h"

using namespace atune;

#define FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP (1)
#define FLAGMASK_PID_VALUES_READY (2)

#define WITHIN_CONFIGURED_PCT_OF(val1, desired, pct) (abs(val1 - desired) < (desired * pct))
#define WITHIN_CONFIGURED_PCT_OF_CENTER_TARGET() WITHIN_CONFIGURED_PCT_OF(last_measurement, this->cfg.center_value, this->cfg.centerline_allowed_error)

void GenericAutotune_t::init(AutotuneConfig_t cfg) {
  this->cfg = cfg;
  LOG_INFO("Beginning autotune, target=");
  LOG_INFO(this->cfg.center_value);
  LOG_INFO("; band=");
  LOG_INFO(this->cfg.hysteresis_distance);
  LOG_INFO("\n");
  this->flags = 0;
  this->cycles_completed = -1; // Start at -1 to ensure first rampup will not be counted as a cycle
  this->state = atune::BEGIN_ATUNE;
  this->latest_duty_cycle = 0;
  this->last_time_decreased_past_middle = 0;
  this->last_time_increased_past_middle = 0;
} // GenericAutotune_t::autotuneInit

void GenericAutotune_t::beginTabularReporting(TabularSource_t src_id, uint16_t interval) {
  this->tabular_obj.init(src_id,
                         sizeof(this->tabular_ptr_arr) / sizeof(this->tabular_ptr_arr[0]), this->tabular_ptr_arr,
                         F("computed_p,computed_i,computed_d,latest_duty_cycle,output_min_compensated,output_max_compensated,min_measured_in_cycle,max_measured_in_cycle,average_oscillation_min,average_oscillation_max,cfg.output_ptr,last_time_increased_past_middle,last_time_decreased_past_middle,average_oscillation_period,cycles_completed,state"));
  this->tabular_obj.setLoggingInterval(interval);
} // beginTabularReporting

void GenericAutotune_t::endTabularReporting() {
  this->tabular_obj.stop();
} // endTabularReporting

bool GenericAutotune_t::computeDutyCycleAndCompensate() {
  uint32_t on_time, off_time, now = millis();
  off_time = now - this->last_time_decreased_past_middle;
  on_time = now - this->last_time_increased_past_middle - off_time;
  this->latest_duty_cycle = ((float)(on_time)) / ((float)on_time + off_time);

  if (WITHIN_CONFIGURED_PCT_OF(this->latest_duty_cycle, 0.5, this->cfg.duty_allowed_error)) {
    return false; // duty cycle is 'good enough', done with compensation
  }

  /*
   * Duty cycle was not close enough to 50%, need to change the output values to
   * bring it closer.
   *
   * For simplicity:
   * - A too-high duty cycle will result in a decrease in output maximum.
   * - A too-low duty will result in an increase in the minimum.
   */
  if (this->latest_duty_cycle < 0.5) {
    this->output_min_compensated += (this->output_max_compensated - this->output_min_compensated) * this->cfg.compensation_rate;
  } else {
    this->output_max_compensated -= (this->output_max_compensated - this->output_min_compensated) * this->cfg.compensation_rate;
  }

  if (this->output_min_compensated >= this->output_max_compensated) {
    LOG_ERROR("Generic autotune failed: compensated minimum reached/surpassed compensated maximum");
    this->stop();
  }
  if (this->output_max_compensated <= this->output_min_compensated) {
    LOG_ERROR("Generic autotune failed: compensated maximum reached/surpassed compensated minimum");
    this->stop();
  }

  return true;
} // GenericAutotune_t::computeDutyCycleAndCompensate

void GenericAutotune_t::computePidValsFromAverages() {
  float ultimate_period, ultimate_gain;
  // NOTE: Seconds chosen here to avoid values getting very large or small -
  //       this may only be valid for temps, and possibly not for other data,
  //       but since these are floats it should be good enough. Always double
  //       check the debug output.
  ultimate_period = ((float)this->average_oscillation_period) / 1000;
  ultimate_gain = (4.0 / M_PI) * (this->output_max_compensated - this->output_min_compensated) / (this->average_oscillation_max - this->average_oscillation_min);

  // Computed based on Ziegler–Nichols method, when values were measured with Relay method
  // (all accrding to this:) https://en.wikipedia.org/wiki/PID_controller
  computed_p = 0.6 * ultimate_gain;
  computed_i = 1.2 * (ultimate_gain / ultimate_period);
  computed_d = (3.0 / 40.0) * (ultimate_gain * ultimate_period);

  LOG_DEBUG("P="); LOG_DEBUG(computed_p); LOG_DEBUG("\n");
  LOG_DEBUG("I="); LOG_DEBUG(computed_i); LOG_DEBUG("\n");
  LOG_DEBUG("D="); LOG_DEBUG(computed_d); LOG_DEBUG("\n");

  this->flags |= FLAGMASK_PID_VALUES_READY;
} // GenericAutotune_t::computePidValsFromAverages

bool GenericAutotune_t::getPid(double *p, double *i, double *d) {
  (*p) = computed_p;
  (*i) = computed_i;
  (*d) = computed_d;
  return this->flags & FLAGMASK_PID_VALUES_READY;
} // GenericAutotune_t::getPid

bool GenericAutotune_t::runtime(double last_measurement) {
  switch (this->state) {
  case STOPPED:
    return this->flags & FLAGMASK_PID_VALUES_READY;
    break;
  case BEGIN_ATUNE:
    this->output_min_compensated = this->cfg.output_min_allowable;
    this->output_max_compensated = this->cfg.output_max_allowable;
    this->min_measured_in_cycle = 0;
    this->max_measured_in_cycle = 0;
    this->average_oscillation_period = 0;
    this->average_oscillation_min = 0;
    this->average_oscillation_max = 0;
    this->state = RAMPUP;
    break;
  case RAMPUP:
    if (last_measurement > this->cfg.center_value - this->cfg.hysteresis_distance) {
      this->state = BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH;
    } else {
      (*this->cfg.output_ptr) = this->cfg.output_max_allowable;
    }
    break;
  case BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH:
    // Currently trying to increase the input
    (*this->cfg.output_ptr) = this->output_max_compensated;

    if (last_measurement >= this->cfg.center_value + this->cfg.hysteresis_distance) {
      this->state = BANGBANG_DUTY_COMPENSATION_OUTPUT_LOW;
      break;
    }

    if (WITHIN_CONFIGURED_PCT_OF_CENTER_TARGET()) {
      // Reached within acceptable error of center value; raise a flag for later
      this->flags |= FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP;
    } else if (this->flags & FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP) {
      // We are not in the center, but we were last time.
      this->flags &= ~FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP;
      this->cycles_completed++;
      if (this->cycles_completed >= ATUNE_CYCLES_REQUIRED_AT_STARTUP) {
        // We can safely compute the duty cycle; do it before storing last_time_increased_past_middle
        if (!this->computeDutyCycleAndCompensate()) {
          // Compensation done, so we're done with adjsuting duty cycle
          this->cycles_completed = -1;
          this->state = OSCILLATE_WHILE_READING;
        }
      }
      this->last_time_increased_past_middle = millis(); // Store the time so duty can be found
    }
    break;
  case BANGBANG_DUTY_COMPENSATION_OUTPUT_LOW:
    // Currently trying to increase the input
    (*this->cfg.output_ptr) = this->output_min_compensated;

    if (last_measurement <= this->cfg.center_value - this->cfg.hysteresis_distance) {
      this->state = BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH;
      break;
    }

    if (WITHIN_CONFIGURED_PCT_OF_CENTER_TARGET()) {
      // Reached within acceptable error of center value; raise a flag for later
      this->flags |= FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP;
    } else if (this->flags & FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP) {
      // We are not in the center, but we were last time.
      this->flags &= ~FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP;
      this->last_time_decreased_past_middle = millis(); // Store the time so duty can be found
    }
    break;
  case OSCILLATE_WHILE_READING:
    // Measuring the input oscillations and peaks, while also driving the ouptut high

    // First, update the output in case we exited hysteresis
    if (last_measurement <= this->cfg.center_value - this->cfg.hysteresis_distance) {
      // Below hysteresis
      (*this->cfg.output_ptr) = this->output_max_compensated;
    } else if (last_measurement >= this->cfg.center_value + this->cfg.hysteresis_distance) {
      // Above hysteresis; this is now considered the start of the cycle
      (*this->cfg.output_ptr) = this->output_min_compensated;
      this->flags |= FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP;
    } else if (this->flags & FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP) {
      // Just exited max hysteresis; consider that the end of the cycle and set/reset values accordingly
      this->flags &= ~FLAGMASK_WITHIN_CYCLE_END_AT_LAST_STEP; // Clear flag so this happens once
      if (cycles_completed >= 0) {
        // At end of first cycle, it's safe to store data
        this->average_oscillation_min += this->min_measured_in_cycle;
        this->average_oscillation_max += this->max_measured_in_cycle;
        this->average_oscillation_period += millis() - this->last_time_increased_past_middle;
      }
      this->min_measured_in_cycle = FLT_MAX;
      this->max_measured_in_cycle = FLT_MIN;
      this->last_time_increased_past_middle = millis();
      this->cycles_completed++;
      if (this->cycles_completed >= ATUNE_CYCLES_REQUIRED_FOR_MEASUREMENT) {
        this->state = COMPUTE_AND_STORE_DATA;
      }
    }

    // Check for min/max and store it in averages
    if (last_measurement > this->max_measured_in_cycle) this->max_measured_in_cycle = last_measurement;
    if (last_measurement < this->min_measured_in_cycle) this->min_measured_in_cycle = last_measurement;
    break;
  case COMPUTE_AND_STORE_DATA:
    this->average_oscillation_min /= cycles_completed;
    this->average_oscillation_max /= cycles_completed;
    this->average_oscillation_period /= cycles_completed;
    LOG_DEBUG("Autotune reached end successfully:\n");
    LOG_DEBUG("\tCYCLES      = "); LOG_DEBUG(this->cycles_completed); LOG_DEBUG("\n");
    LOG_DEBUG("\tAVG OSC MIN = "); LOG_DEBUG(this->average_oscillation_min); LOG_DEBUG("\n");
    LOG_DEBUG("\tAVG OSC MAX = "); LOG_DEBUG(this->average_oscillation_max); LOG_DEBUG("\n");
    LOG_DEBUG("\tAVG OSC PER = "); LOG_DEBUG(this->average_oscillation_period); LOG_DEBUG("\n");

    this->computePidValsFromAverages();
    this->state = STOPPED;
    break;
  } // switch
  return false;
} // GenericAutotune_t::autotuneRuntime

void GenericAutotune_t::stop() {
  LOG_INFO("stopping autotune\n");
  this->state = STOPPED;
} // GenericAutotune_t::autotuneStop
