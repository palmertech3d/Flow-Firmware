/**
 * @Author: Nick Steele
 * @Date:   21:26 Sep 09 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 22:01 Oct 07 2021
 */

// #include "logger.h"

#ifndef UNIT_TESTING_H
#define UNIT_TESTING_H

#ifdef UNIT_LEVEL_TESTING

#define TEST_PASSED_LOG_FL(str, file, line) do{ Serial.print(F("PASSED (" file ":" MACRO_STRIZE(line) "): " str "\n")); }while(0)
#define TEST_PASSED_LOG(str) TEST_PASSED_LOG_FL(str, __FILE__, __LINE__)

#define TEST_FAILED_LOG_FL(str, file, line, valx, valy) do{ Serial.print(F("FAILED! (" file ":" MACRO_STRIZE(line) "): " str " (first: ")); Serial.print(valx); Serial.print(F(", second: ")); Serial.print(valy); Serial.print(F(")\n")); }while(0)
#define TEST_FAILED_LOG(str, valx, valy) TEST_FAILED_LOG_FL(str, __FILE__, __LINE__, valx, valy)
#define TEST_SINGLE_VAL_FAILED_LOG(str, valx) TEST_FAILED_LOG_FL(str, __FILE__, __LINE__, valx)

#define TEST_ASSERT(x, test_result) TEST_ASSERT_EQUAL(x, true, test_result)
#define TEST_ASSERT_EQUAL(x, y, test_result) do{ test_result.total++; if ((x) != (y)) { TEST_FAILED_LOG(#x " not equal to "  #y, x, y); test_result.failed++; } else {TEST_PASSED_LOG(#x " equal to "  #y);}}while(0)
#define TEST_ASSERT_NEQUAL(x, y, test_result) do{ test_result.total++; if ((x) == (y)) { TEST_FAILED_LOG(#x " equal to "  #y, x, y); test_result.failed++; } else {TEST_PASSED_LOG(#x " not equal to "  #y);}}while(0)
#define TEST_ASSERT_WITHIN_PCT(x, y, pct, test_result) do{ test_result.total++; if (abs(x - y) >= (((float)y) * pct)) { TEST_FAILED_LOG(#x " not within (" #pct "*100%) of "  #y, x, y); test_result.failed++; } else {TEST_PASSED_LOG(#x " within (" #pct "*100%) of "  #y);}}while(0)

typedef struct TestResult_struct {
  uint16_t failed = 0;
  uint16_t total = 0;
} TestResult_t;

#endif /* ifdef UNIT_LEVEL_TESTING */
#endif /* end of include guard: UNIT_TESTING_H */
