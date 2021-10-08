/**
 * @Author: Nick Steele
 * @Date:   22:12 Oct 06 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 18:21 Oct 08 2021
 */

#include "config_defaults.h"
#include "tabular/tabular.h"
#include "tabular/delegator-obj.h"

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
  this->ptr_arr_len_minus_one = ptr_arr_len - 1; // Do this once, since the final element is handled seperately (see logIfNeeded)
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

void Tabular_t::logDataPoint(TabularData_t *tabular_ptr, bool print_comma) {
  // Generated using gen-tabular.py
  // Generated using gen-tabular.py
  switch (tabular_ptr->fmt) {
  case FMT_FLOAT:
    LOG_DATA(*tabular_ptr->ptr.float_ptr);
    break;
  case FMT_DOUBLE:
    LOG_DATA(*tabular_ptr->ptr.double_ptr);
    break;
  case FMT_UINT8_T:
    LOG_DATA(*tabular_ptr->ptr.uint8_t_ptr);
    break;
  case FMT_UINT16_T:
    LOG_DATA(*tabular_ptr->ptr.uint16_t_ptr);
    break;
  case FMT_UINT32_T:
    LOG_DATA(*tabular_ptr->ptr.uint32_t_ptr);
    break;
  case FMT_INT8_T:
    LOG_DATA(*tabular_ptr->ptr.int8_t_ptr);
    break;
  case FMT_INT16_T:
    LOG_DATA(*tabular_ptr->ptr.int16_t_ptr);
    break;
  case FMT_INT32_T:
    LOG_DATA(*tabular_ptr->ptr.int32_t_ptr);
    break;
  } // switch(tabular_ptr->fmt)
  if (print_comma)
    LOG_DATA(',');
} // Tabular_t::logDataPoint

void Tabular_t::resetLogCountdown() {
  this->next_log_time = millis() + this->interval;
} // Tabular_t::resetLogCountdown

void Tabular_t::logIfNeeded() {
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
    for (uint8_t i = 0; i < this->ptr_arr_len_minus_one; i++) {
      this->logDataPoint(this->ptr_arr + i);
    }
    this->logDataPoint(this->ptr_arr + this->ptr_arr_len_minus_one, false);
    LOG_DATA('\n');
  }
} // Tabular_t::logIfNeeded

// TEST FUNCTIONS /////////////////////////////////////////////////////////////

#ifdef UNIT_LEVEL_TESTING

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
  return accumulator;
} // Heater::TEST_heater

#endif // ifdef UNIT_LEVEL_TESTING
