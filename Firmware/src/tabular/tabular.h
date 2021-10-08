/**
 * @Author: Nick Steele
 * @Date:   21:59 Oct 06 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 19:22 Oct 08 2021
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

typedef struct TabularCallback_struct {
  union {
    float float_val;
    double double_val;
    uint8_t uint8_t_val;
    uint16_t uint16_t_val;
    uint32_t uint32_t_val;
    int8_t int8_t_val;
    int16_t int16_t_val;
    int32_t int32_t_val;
  } val;
  enum _TabularDataFmt_enum fmt;
} TabularCallback_t;

using TabularCallback_f = TabularCallback_t (*)(uint8_t);

#define _TABULAR_SOURCES_COUNT 4

typedef enum TabularSource_enum {
  TDS_NONE_         = 0,
  TDS_CORE          = 1,
  TDS_HEATER        = 2,
  TDS_MOTOR         = 3,
  TDS_TEMP_ATUNE    = 4
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
TabularCallback_f dataAccessorCallback;

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

/**
 * Same as the other, but for a callback's returned data.
 *
 * @param callback_data [description]
 * @param print_comma   [description]
 */
void logDataPoint(TabularCallback_t callback_data, bool print_comma = true);

public:

Tabular_t();

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
 * Creates a tabular which uses a callback to get the data to be printed. Use
 * this only when you cannot access the pointers to the data directly.
 *
 * As its first and only argument, The callback will recieve the zero-based
 * index of the column of data to return.
 *
 * @param src           The source's unique ID which is used in the GCODE communication.
 * @param col_count     Number of columns to call the callback for
 * @param callback      Callback to run when data is needed, called col_count times
 * @param colnames      Flash string defining what the columns should be named.
 *                      Must be CSV formatted
 */
void init(TabularSource_t src, uint8_t col_count, TabularCallback_t (*callback)(uint8_t), const __FlashStringHelper *colnames);

/**
 * Set the logging interval. The default is set in the constructor.
 *
 * @param period_ms The period to log in milliseconds.
 */
void setLoggingInterval(uint16_t period_ms);

/**
 * Stop reporting. Must call init again to enable again.
 *
 * @param tabular_ptr [description]
 * @param src         [description]
 */
void stop();

friend class TabularDelegator_t;
friend class TabularTester_t;
} // class Tabular_t

;

class TabularDelegator_t {
private:
Tabular_t *registered_tabulars[_TABULAR_SOURCES_COUNT];

/**
 * Make the delegator aware that the tabular instance needs to be pinged
 * regularly so it can print periodically
 *
 * @param tabular_ptr [description]
 * @param src         [description]
 */
void registerTabular(Tabular_t *tabular_ptr, TabularSource_t src);

/**
 * Stop pinging the tabular instance.
 *
 * @param tabular_ptr [description]
 * @param src         [description]
 */
void unregister(Tabular_t *tabular_ptr, TabularSource_t src);

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
