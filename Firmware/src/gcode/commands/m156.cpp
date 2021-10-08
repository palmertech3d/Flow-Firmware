/**
 * @Author: Nick Steele
 * @Date:   7:58 Oct 08 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 18:22 Oct 08 2021
 */

 #include "gcode/gcode.h"
 #include "tabular/tabular.h"
 #include "hardware/heater.h"

void GcodeExecuter::m156(GcodeCommand_t cmd){
  GcodeArg_t source_gc = gcodeParseValueFor('c', cmd);
  if (source_gc == GCODE_ARG_VALUE_ERR) {
    LOG_WARN("No source selected\n");
    return;
  };
  TabularSource_t source = (TabularSource_t)((uint16_t)source_gc);
  GcodeArg_t rate = gcodeParseValueFor('r', cmd);
  if (rate == GCODE_ARG_VALUE_ERR) {
    LOG_INFO("Stopping reporter #");
    LOG_INFO((uint16_t)source_gc);
    LOG_INFO('\n');
    switch (source) {
    case TDS_CORE:
      break;
    case TDS_HEATER:
      Heater::disableReporter();
      break;
    case TDS_MOTOR:
      break;
    case TDS_TEMP_ATUNE:
      Heater::disableAtuneReporter();
      break;
    default:
      LOG_WARN("Invalid tabular reporter selected\n");
    } // switch
  } else {
    switch (source) {
    case TDS_CORE:
      break;
    case TDS_HEATER:
      Heater::enableReporter(rate);
      break;
    case TDS_MOTOR:
      break;
    case TDS_TEMP_ATUNE:
      Heater::enableAtuneReporter(rate);
      break;
    default:
      LOG_WARN("Invalid tabular reporter selected\n");
    } // switch
  }
} // GcodeExecuter::m106
