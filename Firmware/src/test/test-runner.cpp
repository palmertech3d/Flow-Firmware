/**
 * @Author: Nick Steele
 * @Date:   21:35 Sep 09 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 20:22 Oct 08 2021
 */

#include "test-runner.h"

#ifdef UNIT_LEVEL_TESTING

#include <Arduino.h>
#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "hardware/heater.h"
#include "hardware/motor.h"
#include "tabular/tabular.h"
#include "abstract_logic/autotune.h"

void resultAccumulatorAdd(TestResult_t new_values, TestResult_t *accumulator) {
  accumulator->total += new_values.total;
  accumulator->failed += new_values.failed;
} // resultAccumulatorAdd

void runUnitTestsThenLoop() {
  TestResult_t accumulator;

  // TEST FUNCTIONS ///////////////////////////////////////////////////////////
  atune::GenericAutotune_t at;
  resultAccumulatorAdd(at.TEST_autotune(), &accumulator);
  resultAccumulatorAdd(TEST_gcode(), &accumulator);
  resultAccumulatorAdd(Heater::TEST_heater(), &accumulator);
  resultAccumulatorAdd(Heater::TEST_runaway(), &accumulator);
  resultAccumulatorAdd(Motor::TEST_preventColdExtrusion(), &accumulator);
  GcodeParser parser;
  resultAccumulatorAdd(parser.TEST_gcodeParser(), &accumulator);
  TabularTester_t tabtest;
  resultAccumulatorAdd(tabtest.TEST_tabular(), &accumulator);

  // RESULTS OUTPUT ///////////////////////////////////////////////////////////
  LOG_INFO(F("################################################################################\n"));
  LOG_INFO(F("TEST RESULTS:\n"));
  LOG_INFO(F("\tTOTAL: ")); LOG_INFO(accumulator.total); LOG_INFO(F("\n"));
  LOG_INFO(F("\tFAILED: ")); LOG_INFO(accumulator.failed); LOG_INFO(F("\n"));
  LOG_INFO(F("\tPASSED: ")); LOG_INFO(accumulator.total - accumulator.failed); LOG_INFO(F("\n"));
  if (accumulator.failed == 0) {
    LOG_INFO(F("All tests passed :)\n"));
  } else {
    LOG_INFO(F("Some tests failed :'(\n"));
  }
  LOG_INFO(F("Testing complete; entering infinite loop."));
  while (1);
} // runUnitTests

#endif // ifdef UNIT_LEVEL_TESTING
