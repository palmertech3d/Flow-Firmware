/**
 * @Author: Nick Steele
 * @Date:   21:35 Sep 09 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 13:23 Sep 11 2021
 */

#include "test-runner.h"

#ifdef UNIT_LEVEL_TESTING

#include <Arduino.h>
#include "gcode/gcode.h"
#include "hardware/heater.h"

void resultAccumulatorAdd(TestResult_t new_values, TestResult_t *accumulator) {
  accumulator->total += new_values.total;
  accumulator->failed += new_values.failed;
} // resultAccumulatorAdd

void runUnitTestsThenLoop() {
  TestResult_t accumulator;

  // TEST FUNCTIONS ///////////////////////////////////////////////////////////
  resultAccumulatorAdd(TEST_gcode(), &accumulator);
  resultAccumulatorAdd(Heater::TEST_heater(), &accumulator);
  resultAccumulatorAdd(Heater::TEST_runaway(), &accumulator);

  // RESULTS OUTPUT ///////////////////////////////////////////////////////////
  INFO_LOG(F("################################################################################\n"));
  INFO_LOG(F("TEST RESULTS:\n"));
  INFO_LOG(F("\tTOTAL: ")); INFO_LOG(accumulator.total); INFO_LOG(F("\n"));
  INFO_LOG(F("\tFAILED: ")); INFO_LOG(accumulator.failed); INFO_LOG(F("\n"));
  INFO_LOG(F("\tPASSED: ")); INFO_LOG(accumulator.total - accumulator.failed); INFO_LOG(F("\n"));
  if (accumulator.failed == 0) {
    INFO_LOG(F("All tests passed :)\n"));
  } else {
    INFO_LOG(F("Some tests failed :'(\n"));
  }
  INFO_LOG(F("Testing complete; entering infinite loop."));
  while (1);
} // runUnitTests

#endif // ifdef UNIT_LEVEL_TESTING
