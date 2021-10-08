/**
 * @Author: Nick Steele
 * @Date:   21:59 Oct 06 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 7:56 Oct 08 2021
 */

#include <stdint.h>
#include <Arduino.h>
#include "logger.h"
#include "test/unit-testing.h"

#ifndef TABULAR
#define TABULAR

#define _TABULAR_GCODE_RESPONSE_CMD "M155"

// Generated using gen-tabular.py
enum _TabularDataFmt_enum {
  FMT_FLOAT,
  FMT_DOUBLE,
  FMT_UINT8_T,
  FMT_UINT16_T,
  FMT_UINT32_T,
  FMT_INT8_T,
  FMT_INT16_T,
  FMT_INT32_T,
};

typedef struct TabularData_t {
  union {
    float    *float_ptr;
    double   *double_ptr;
    uint8_t  *uint8_t_ptr;
    uint16_t *uint16_t_ptr;
    uint32_t *uint32_t_ptr;
    int8_t   *int8_t_ptr;
    int16_t  *int16_t_ptr;
    int32_t  *int32_t_ptr;
  } ptr;
  enum _TabularDataFmt_enum fmt;
} TabularDataPtr_t;

#define _TABULAR_SOURCES_COUNT 3

typedef enum TabularSource_enum {
  TDS_CORE          = 1,
  TDS_HEATER        = 2,
  TDS_MOTOR         = 4,
  TDS_TEMP_ATUNE    = 8
} TabularSource_t;

class Tabular_t {
private:
uint8_t flags;
TabularSource_t src;
uint16_t interval;
uint32_t next_log_time;
uint8_t ptr_arr_len_minus_one;
TabularData_t *ptr_arr;
const __FlashStringHelper *colnames;

/**
 * Logs data when needed and resets internal countdown
 */
void logIfNeeded();

/**
 * Restart the countdown to the next log
 */
void resetLogCountdown();

/**
 * Logs the given data point by choosing the type to log using
 * @param
 * @param tabular_ptr The pointer and type to log
 * @param print_comma Whether to print a comma after the data (false for final)
 */
void logDataPoint(TabularData_t *tabular_ptr, bool print_comma = true);

public:

/**
 * Creates the tabular with a pointer to a null_ptr terminated array of pointers
 * to all data to be printed.
 *
 * @param src           The source's unique ID which is used in the GCODE communication.
 * @param ptr_arr_len   Length of the ptr_arr
 * @param ptr_arr       The of pointers of all data to be printed
 * @param colnames      A "\0" terminated array of flash strings defining what the columns
 *                      should be named, in order of ptr_arr
 */
void init(TabularSource_t src, uint8_t ptr_arr_len, TabularData_t *ptr_arr, const __FlashStringHelper *colnames);

/**
 * Set the logging interval. The default is set in the constructor.
 *
 * @param period_ms The period to log in milliseconds.
 */
void setLoggingInterval(uint16_t period_ms);

friend class TabularDelegator_t;
friend class TabularTester_t;
} // class Tabular_t

;

class TabularDelegator_t {
private:
Tabular_t *registered_tabulars[_TABULAR_SOURCES_COUNT];

void registerTabular(Tabular_t *tabular_ptr, TabularSource_t src);

public:

TabularDelegator_t();

/**
 * Must be run regularly to guarantee that loggers will be printing around their requested interval.
 * @param
 */
void idle();

friend class Tabular_t;
friend class TabularTester_t;
} // class TabularDelegator_t

;

#ifdef UNIT_LEVEL_TESTING
class TabularTester_t {
public:

TestResult_t TEST_tabular();
} // class TabularTester_t

;

#endif // ifdef UNIT_LEVEL_TESTING

#endif /* end of include guard: TABULAR */
