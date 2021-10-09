/***************************************************
 *  Generated at comilation using gen-tabular.py * *
 ***************************************************/
#include "tabular/tabular.h"

void Tabular_t::logDataPoint(TabularData_t *tabular_ptr, bool print_comma) {
switch(tabular_ptr->fmt) {
case FMT_FLOAT: LOG_DATA(*tabular_ptr->ptr.float_ptr); break;
case FMT_DOUBLE: LOG_DATA(*tabular_ptr->ptr.double_ptr); break;
case FMT_UINT8_T: LOG_DATA(*tabular_ptr->ptr.uint8_t_ptr); break;
case FMT_UINT16_T: LOG_DATA(*tabular_ptr->ptr.uint16_t_ptr); break;
case FMT_UINT32_T: LOG_DATA(*tabular_ptr->ptr.uint32_t_ptr); break;
case FMT_INT8_T: LOG_DATA(*tabular_ptr->ptr.int8_t_ptr); break;
case FMT_INT16_T: LOG_DATA(*tabular_ptr->ptr.int16_t_ptr); break;
case FMT_INT32_T: LOG_DATA(*tabular_ptr->ptr.int32_t_ptr); break;
} // switch(tabular_ptr->fmt)
if (print_comma) LOG_DATA(',');
}


void Tabular_t::logDataPoint(TabularCallback_t callback_data, bool print_comma) {
switch(callback_data.fmt) {
case FMT_FLOAT: LOG_DATA(callback_data.val.float_val); break;
case FMT_DOUBLE: LOG_DATA(callback_data.val.double_val); break;
case FMT_UINT8_T: LOG_DATA(callback_data.val.uint8_t_val); break;
case FMT_UINT16_T: LOG_DATA(callback_data.val.uint16_t_val); break;
case FMT_UINT32_T: LOG_DATA(callback_data.val.uint32_t_val); break;
case FMT_INT8_T: LOG_DATA(callback_data.val.int8_t_val); break;
case FMT_INT16_T: LOG_DATA(callback_data.val.int16_t_val); break;
case FMT_INT32_T: LOG_DATA(callback_data.val.int32_t_val); break;
} // switch(callback_data->fmt)
if (print_comma) LOG_DATA(',');
}
