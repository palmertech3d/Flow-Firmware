/**
 * @Author: Nick Steele
 * @Date:   22:12 Oct 06 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 20:56 Oct 08 2021
 */

#include "config_defaults.h"
#include "tabular/tabular.h"
#include "tabular/delegator-obj.h"
#include "tabular/generated/tabular-generated.h"

#define _TABFLAG_ALREADY_PRINTED_TITLE_MASK 1

Tabular_t::Tabular_t() {
  this->src = TDS_NONE_;
}

void Tabular_t::init(TabularSource_t src, uint8_t ptr_arr_len, TabularData_t *ptr_arr, const __FlashStringHelper *colnames) {
  if (this->src != TDS_NONE_) {
    // Already registered to a source or to the delegator
    LOG_ERROR("Tabular will not init because this->src is not zero.\n");
  }
  this->src = src;
  this->ptr_arr = ptr_arr;
  this->ptr_arr_len_minus_one = ptr_arr_len - 1; // Do this now, since the full qty is never used (see logIfNeeded)
  this->colnames = colnames;

  this->flags = 0;

  this->setLoggingInterval(TABULAR_DEFAULT_LOG_PERIOD_MS);

  tabular_delegator.registerTabular(this, src);
} // Tabular_t::init

void Tabular_t::init(TabularSource_t src, uint8_t col_count, TabularCallback_f accessorCallback, const __FlashStringHelper *colnames) {
  if (this->src != TDS_NONE_) {
    // Already registered to a source or to the delegator
    LOG_ERROR("Tabular will not init because this->src is not zero.\n");
  }
  this->src = src;
  this->ptr_arr = NULL;
  this->dataAccessorCallback = accessorCallback;
  this->ptr_arr_len_minus_one = col_count - 1;
  this->colnames = colnames;

  this->flags = 0;

  this->setLoggingInterval(TABULAR_DEFAULT_LOG_PERIOD_MS);

  tabular_delegator.registerTabular(this, src);
} // Tabular_t::init

void Tabular_t::stop(){
  if (this->src == TDS_NONE_) {
    // Not registered to a source or to the delegator, so there's nothing to stop
    return;
  }
  tabular_delegator.unregister(this, src);
  this->src = TDS_NONE_;
} // Tabular_t::init

void Tabular_t::setLoggingInterval(uint16_t period_ms) {
  this->interval = period_ms;
  this->resetLogCountdown();
} // Tabular_t::setLoggingInterval

void Tabular_t::resetLogCountdown() {
  this->next_log_time = millis() + this->interval;
} // Tabular_t::resetLogCountdown

void Tabular_t::triggerImmediateLogging() {
  this->next_log_time = 0;
} // Tabular_t::triggerImmediateLogging

void Tabular_t::logIfNeeded() {
  if (this->src == TDS_NONE_) {
    return;
  }
  if ((this->flags & _TABFLAG_ALREADY_PRINTED_TITLE_MASK) == 0) {
    // Have not printed title row yet; print that before anything
    this->flags |= _TABFLAG_ALREADY_PRINTED_TITLE_MASK;
    LOG_DATA(F(_TABULAR_GCODE_RESPONSE_CMD " S"));
    LOG_DATA(this->src);
    LOG_DATA(F(" :epoch[ms],"));
    LOG_DATA(this->colnames);
    LOG_DATA('\n');
  }
  if (millis() >= this->next_log_time) {
    this->resetLogCountdown();
    LOG_DATA(F(_TABULAR_GCODE_RESPONSE_CMD " S"));
    LOG_DATA(this->src);
    LOG_DATA(F(" :"));
    LOG_DATA(millis());
    LOG_DATA(',');

    if (this->ptr_arr != NULL) {
      for (uint8_t i = 0; i < this->ptr_arr_len_minus_one; i++) {
        this->logDataPoint(this->ptr_arr + i);
      }
      this->logDataPoint(this->ptr_arr + this->ptr_arr_len_minus_one, false);
      LOG_DATA('\n');
    } else {
      // Using a callback
      for (uint8_t i = 0; i < this->ptr_arr_len_minus_one; i++) {
        this->logDataPoint(this->dataAccessorCallback(i));
      }
      this->logDataPoint(this->dataAccessorCallback(this->ptr_arr_len_minus_one), false);
      LOG_DATA('\n');
    }
  }
} // Tabular_t::logIfNeeded

// TEST FUNCTIONS /////////////////////////////////////////////////////////////

#ifdef UNIT_LEVEL_TESTING
uint8_t last_col_requested = 255;
uint8_t went_to_default = 0;
uint8_t requested_col_0 = 0;
uint8_t requested_col_1 = 0;
uint8_t requested_col_2 = 0;

TabularCallback_t tabularCallbackTestFunction(uint8_t col) {
  TabularCallback_t result;
  last_col_requested = col;
  switch (col) {
  case 0:
    requested_col_0 = 1;
    result.fmt = FMT_FLOAT;
    result.val.float_val = 1.11;
    break;
  case 1:
    requested_col_1 = 1;
    result.fmt = FMT_UINT8_T;
    result.val.uint8_t_val = 222;
    break;
  case 2:
    requested_col_2 = 1;
    result.fmt = FMT_UINT16_T;
    result.val.uint16_t_val = 333;
    break;
  default:
    went_to_default = 1;
    result.fmt = FMT_UINT8_T;
    result.val.uint8_t_val = 0;
  } // switch
  return result;
} // TabularCallbackTestFunction

TestResult_t TabularTester_t::TEST_tabular() {
  TestResult_t accumulator;

  float float_1 = 0.1, float_2 = 0.2, float_3 = 0.3;
  uint8_t uint8_t_1 = 11, uint8_t_2 = 22, uint8_t_3 = 33;
  int32_t int32_t_1 = -111, int32_t_2 = -222, int32_t_3 = -333;
  TabularData_t ptr_arr[9] = {
    {.ptr = {.float_ptr = &float_1}, .fmt = FMT_FLOAT},
    {.ptr = {.float_ptr = &float_2}, .fmt = FMT_FLOAT},
    {.ptr = {.float_ptr = &float_3}, .fmt = FMT_FLOAT},

    {.ptr = {.uint8_t_ptr = &uint8_t_1}, .fmt = FMT_UINT8_T},
    {.ptr = {.uint8_t_ptr = &uint8_t_2}, .fmt = FMT_UINT8_T},
    {.ptr = {.uint8_t_ptr = &uint8_t_3}, .fmt = FMT_UINT8_T},

    {.ptr = {.int32_t_ptr = &int32_t_1}, .fmt = FMT_INT32_T},
    {.ptr = {.int32_t_ptr = &int32_t_2}, .fmt = FMT_INT32_T},
    {.ptr = {.int32_t_ptr = &int32_t_3}, .fmt = FMT_INT32_T}};

  Tabular_t tabular_obj = Tabular_t();
  tabular_obj.init(TDS_CORE, 9, ptr_arr, F("float_1,float_2,float_3,uint8_t_1,uint8_t_2,uint8_t_3,int32_1,int32_2,int32_3"));
  tabular_obj.setLoggingInterval(100);
  TEST_ASSERT_EQUAL(tabular_obj.src, TDS_CORE, accumulator);

  // Title not logged yet
  TEST_ASSERT_EQUAL((tabular_obj.flags & _TABFLAG_ALREADY_PRINTED_TITLE_MASK) == 0, true, accumulator);
  delay(50); // Wait some time to help checking that the data was not logged yet
  tabular_delegator.idle();
  // Title has been logged
  TEST_ASSERT_EQUAL((tabular_obj.flags & _TABFLAG_ALREADY_PRINTED_TITLE_MASK) != 0, true, accumulator);
  // Data was not logged; check by making sure it's 'about' to log
  TEST_ASSERT_EQUAL(tabular_obj.next_log_time < millis() + 75, true, accumulator);
  delay(60); // Delay plenty of time to log data
  tabular_delegator.idle();
  // Make sure it logged this time
  TEST_ASSERT_EQUAL(tabular_obj.next_log_time > millis() + 75, true, accumulator);
  delay(100); // Make sure it logs again
  tabular_delegator.idle();
  TEST_ASSERT_EQUAL(tabular_obj.next_log_time > millis() + 75, true, accumulator);

  // Test immediately triggered logging
  tabular_obj.setLoggingInterval(1000);
  tabular_obj.resetLogCountdown();
  delay(500);
  tabular_delegator.idle();
  TEST_ASSERT((tabular_obj.next_log_time < millis() + 550), accumulator); // countdown not reset
  // Unless triggered, it shouldn't log until after the second ends, so trigger it now
  tabular_obj.triggerImmediateLogging();
  tabular_delegator.idle();
  TEST_ASSERT((tabular_obj.next_log_time > millis() + 900), accumulator); // countdown was indeed reset

  tabular_obj.setLoggingInterval(100);
  tabular_obj.resetLogCountdown();
  delay(100);
  tabular_delegator.idle();
  tabular_obj.stop();
  delay(100); // Make sure it does not log again
  tabular_delegator.idle();
  TEST_ASSERT_EQUAL((tabular_obj.next_log_time < millis()), true, accumulator);

  tabular_obj.init(TDS_CORE, 3, &tabularCallbackTestFunction, F("func_result1, func_result2, func_result3"));
  tabular_obj.setLoggingInterval(50);

  last_col_requested = 255;
  delay(50); tabular_delegator.idle();
  TEST_ASSERT_EQUAL(tabular_obj.next_log_time > millis(), true, accumulator);
  TEST_ASSERT_EQUAL(requested_col_0, 1, accumulator);
  TEST_ASSERT_EQUAL(requested_col_1, 1, accumulator);
  TEST_ASSERT_EQUAL(requested_col_2, 1, accumulator);
  TEST_ASSERT_EQUAL(last_col_requested, 2, accumulator);
  TEST_ASSERT_EQUAL(went_to_default, 0, accumulator);

  last_col_requested = 255;
  delay(50); tabular_delegator.idle();
  TEST_ASSERT_EQUAL(tabular_obj.next_log_time > millis(), true, accumulator);
  TEST_ASSERT_EQUAL(requested_col_0, 1, accumulator);
  TEST_ASSERT_EQUAL(requested_col_1, 1, accumulator);
  TEST_ASSERT_EQUAL(requested_col_2, 1, accumulator);
  TEST_ASSERT_EQUAL(last_col_requested, 2, accumulator);
  TEST_ASSERT_EQUAL(went_to_default, 0, accumulator);

  last_col_requested = 255;
  delay(50); tabular_delegator.idle();
  TEST_ASSERT_EQUAL(tabular_obj.next_log_time > millis(), true, accumulator);
  TEST_ASSERT_EQUAL(requested_col_0, 1, accumulator);
  TEST_ASSERT_EQUAL(requested_col_1, 1, accumulator);
  TEST_ASSERT_EQUAL(requested_col_2, 1, accumulator);
  TEST_ASSERT_EQUAL(last_col_requested, 2, accumulator);
  TEST_ASSERT_EQUAL(went_to_default, 0, accumulator);
  return accumulator;
} // Heater::TEST_heater

#endif // ifdef UNIT_LEVEL_TESTING
