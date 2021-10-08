/**
 * @Author: Nick Steele
 * @Date:   20:06 Oct 07 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 23:52 Oct 07 2021
 */
#include "abstract_logic/autotune.h"

#ifdef UNIT_LEVEL_TESTING

using namespace atune;

TestResult_t GenericAutotune_t::TEST_autotune(){
  TestResult_t accumulator;
  GenericAutotune_t at;
  AutotuneConfig_t cfg;
  double at_out = 0;

  cfg.center_value = 100;
  cfg.hysteresis_distance = 10; // 100 +- 10
  cfg.output_min_allowable = 0;
  cfg.output_max_allowable = 1000;
  cfg.output_ptr = &at_out;
  cfg.centerline_allowed_error = 0.01;
  cfg.duty_allowed_error = 0.1;
  cfg.compensation_rate = 0.1;

  at.init(cfg);
  TEST_ASSERT_EQUAL(int(at_out), 0, accumulator);
  TEST_ASSERT_EQUAL(at.last_time_increased_past_middle, 0, accumulator);
  TEST_ASSERT_EQUAL(at.last_time_decreased_past_middle, 0, accumulator);

  TEST_ASSERT_EQUAL(at.runtime(30), false, accumulator); // False when data not ready
  at.runtime(30);
  TEST_ASSERT_EQUAL(int(at_out), cfg.output_max_allowable, accumulator);
  for (uint8_t i = 0; i < 100; i++) {
    at.runtime(30); // Make sure output and state do not change while ramping up
  }
  TEST_ASSERT_EQUAL(int(at_out), cfg.output_max_allowable, accumulator);
  TEST_ASSERT_EQUAL(at.state, atune::RAMPUP, accumulator);
  for (uint8_t i = 0; i < cfg.center_value - cfg.hysteresis_distance + 2; i++) {
    at.runtime(i);
  }
  TEST_ASSERT_EQUAL(at.state, atune::BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH, accumulator);
  TEST_ASSERT_EQUAL(int(at_out), cfg.output_max_allowable, accumulator);
  at.runtime(94); // Pass through center slowly so the timer can catch it
  at.runtime(100);
  at.runtime(106); // Exit center
  TEST_ASSERT_EQUAL(at.state, atune::BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH, accumulator);
  TEST_ASSERT(at.last_time_increased_past_middle > 0, accumulator);
  at.runtime(111); // Exit upper bound of hysteresis
  TEST_ASSERT_EQUAL(at.state, atune::BANGBANG_DUTY_COMPENSATION_OUTPUT_LOW, accumulator);
  at.runtime(111); // Run one more cycle to allow it to change the output
  TEST_ASSERT_EQUAL(at.state, atune::BANGBANG_DUTY_COMPENSATION_OUTPUT_LOW, accumulator);
  delay(200); // Delay so that the duty cycle can be controlled
  TEST_ASSERT_EQUAL(int(at_out), cfg.output_min_allowable, accumulator);
  TEST_ASSERT_EQUAL(at.latest_duty_cycle, 0, accumulator);
  at.runtime(106); // Run two cycles going past center slowly
  at.runtime(100);
  at.runtime(94);
  TEST_ASSERT(at.last_time_decreased_past_middle > 0, accumulator);
  at.runtime(89);
  at.runtime(89);
  TEST_ASSERT_EQUAL(at.state, atune::BANGBANG_DUTY_COMPENSATION_OUTPUT_HIGH, accumulator);
  TEST_ASSERT_EQUAL(int(at_out), cfg.output_max_allowable, accumulator);
  TEST_ASSERT_EQUAL(at.latest_duty_cycle, 0, accumulator);
  at.runtime(94);
  at.runtime(100);
  at.runtime(106);
  at.runtime(111);
  at.runtime(111);
  delay(400); // PEAK #2
  at.runtime(106);
  at.runtime(100);
  at.runtime(94);
  at.runtime(89);
  at.runtime(89);
  delay (200); // TROUGH #2
  at.runtime(94);
  at.runtime(100);
  TEST_ASSERT_EQUAL(at.latest_duty_cycle, 0, accumulator);
  TEST_ASSERT_EQUAL(at.output_min_compensated, cfg.output_min_allowable, accumulator);
  TEST_ASSERT_EQUAL(at.output_max_compensated, cfg.output_max_allowable, accumulator);
  at.runtime(106); // Duty cycle should be stored right here
  TEST_ASSERT_WITHIN_PCT(at.latest_duty_cycle, 0.66, 0.10, accumulator); // Time is a bit innacurate
  TEST_ASSERT_EQUAL(at.output_min_compensated, cfg.output_min_allowable, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.output_max_compensated, 900, 0.005, accumulator);
  at.runtime(111);
  at.runtime(111);
  delay(200); // PEAK #3
  at.runtime(106);
  at.runtime(100);
  at.runtime(94);
  at.runtime(89);
  at.runtime(89);
  delay (400); // TROUGH #3
  at.runtime(94);
  at.runtime(100);
  TEST_ASSERT_WITHIN_PCT(at.latest_duty_cycle, 0.66, 0.10, accumulator); // Should not have changed
  at.runtime(106); // Duty cycle should be stored right here
  // Time will be fairly inaccurate, just check that it's under the midpoint
  TEST_ASSERT((at.latest_duty_cycle < 45), accumulator);
  TEST_ASSERT_WITHIN_PCT(at.output_min_compensated, 90, 0.005, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.output_max_compensated, 900, 0.005, accumulator);
  at.runtime(111);
  at.runtime(111);
  // Make these delays extra long to ensure better timing accuracy to 50% duty cycle
  delay(500); // PEAK #4
  at.runtime(106);
  at.runtime(100);
  at.runtime(94);
  at.runtime(89);
  at.runtime(89);
  delay (500); // TROUGH #4
  at.runtime(94);
  at.runtime(100);
  at.runtime(106); // Duty cycle should be stored right here
  TEST_ASSERT_WITHIN_PCT(at.latest_duty_cycle, 0.50, cfg.duty_allowed_error, accumulator);
  TEST_ASSERT_EQUAL(at.state, atune::OSCILLATE_WHILE_READING, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.output_min_compensated, 90, 0.005, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.output_max_compensated, 900, 0.005, accumulator);
  for (uint8_t i = 0; i < ATUNE_CYCLES_REQUIRED_FOR_MEASUREMENT + 1; i++) {
    at.runtime(111);
    delay(100); // PEAK
    at.runtime(106);
    at.runtime(100);
    at.runtime(94);
    at.runtime(89);
    at.runtime(89);
    delay (100); // TROUGH
    at.runtime(94);
    at.runtime(100);
    at.runtime(106);
    at.runtime(111); // Cycle data should be stored right here
  }
  TEST_ASSERT_EQUAL(at.state, atune::STOPPED, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.average_oscillation_min, 89, 0.005, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.average_oscillation_max, 111, 0.005, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.average_oscillation_period, 200, 0.05, accumulator);
  TEST_ASSERT_EQUAL(at.runtime(111), true, accumulator);

  // THEORETICAL VALUES:
  // Ku = 49.878
  // Kp = 0.200 [seconds]
  TEST_ASSERT_WITHIN_PCT(at.computed_p, 28.127, 0.05, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.computed_i, 279.87, 0.05, accumulator);
  TEST_ASSERT_WITHIN_PCT(at.computed_d, 0.703, 0.05, accumulator);

  return accumulator;
} // TEST_autotune

#endif // ifdef UNIT_LEVEL_TESTING
