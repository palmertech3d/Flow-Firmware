/**
 * @Author: Nick Steele
 * @Date:   21:26 Sep 09 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 21:49 Sep 09 2021
 */

// #include "logger.h"

#ifndef UNIT_TESTING_H
#define UNIT_TESTING_H

#ifdef UNIT_LEVEL_TESTING

#define TEST_PASSED_LOG_FL(str, file, line) do{ Serial.print(F("PASSED (" file ":" MACRO_STRIZE(line) "): " str "\n")); }while(0)
#define TEST_PASSED_LOG(str) TEST_PASSED_LOG_FL(str, __FILE__, __LINE__)

#define TEST_FAILED_LOG_FL(str, file, line) do{ Serial.print(F("FAILED! (" file ":" MACRO_STRIZE(line) "): " str "\n")); }while(0)
#define TEST_FAILED_LOG(str) TEST_FAILED_LOG_FL(str, __FILE__, __LINE__)

#define TEST_ASSERT_EQUAL(x, y, test_result) do{ test_result.total++; if (x != y) { TEST_FAILED_LOG(#x " not equal to "  #y); test_result.failed++; } else {TEST_PASSED_LOG(#x " equal to "  #y);}}while(0)
#define TEST_ASSERT_NEQUAL(x, y, test_result) do{ test_result.total++; if (x == y) { TEST_FAILED_LOG(#x " equal to "  #y); test_result.failed++; } else {TEST_PASSED_LOG(#x " not equal to "  #y);}}while(0)

typedef struct TestResult_struct {
  uint16_t failed = 0;
  uint16_t total = 0;
} TestResult_t;

#endif /* ifdef UNIT_LEVEL_TESTING */
#endif /* end of include guard: UNIT_TESTING_H */
