/**
 * @Author: Nick Steele
 * @Date:   13:35 Sep 05 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 22:25 Oct 06 2021
 */

#ifndef CONFIG_DEFAULTS_H
#define CONFIG_DEFAULTS_H

// MAIN HEATER /////////////////////////////////////////////////////////////////

#define PID_HTR0_P 2
#define PID_HTR0_I 5
#define PID_HTR0_D 1

// HTR thermal runaway
#define HTR0_TR_MAX_TIME_OUT_OF_HYST_MS 60000
#define HTR0_TR_MAX_TIME_OUT_OF_MIN_RAMP_MS 5000
#define HTR0_TR_TEMP_HYSTERESIS_C 10
#define HTR0_TR_MIN_RAMPUP_DEGC_PER_SAMPLE 1
#define HTR0_TR_RAMPUP_SAMPLE_DELAY_MS 20000
// HTR max/min temps
#define HTR0_MAX_TEMP 220
#define HTR0_MIN_TEMP 5

#define MIN_EXTRUSION_TEMP_C 150

// MOTORS /////////////////////////////////////////////////////////////////////

// TODO: Motor speed units
// #define M_EXTRUDER_MAX_SPEED_MILLIRPM_S 1000
// #define M_PULLER_MAX_SPEED_UM_S 5
// #define M_LEVEL_MAX_SPEED_MM_S 5
// #define M_WINDER_MAX_SPEED_MILLIRPM_S 5
//
// #define M_EXTRUDER_MAX_ACCELERATION_MILLIRPM_SS 500
// #define M_PULLER_MAX_ACCELERATION_UM_SS 10
// #define M_LEVEL_MAX_ACCELERATION_MM_SS 10
// #define M_WINDER_MAX_ACCELERATION_MILLIRPM_SS 10

// 100 steps = 2
#define M_EXTRUDER_STEPS_PER_MILLIRPM 53.402
// 60 steps = 14.040 mm/s
#define M_PULLER_STEPS_PER_MM 15.21
#define M_LEVEL_STEPS_PER_MM 130.0 // Untested and probably wrong
// 18 steps: 1.5 RPM
#define M_WINDER_STEPS_PER_MILLIRPM 27.645

#define M_EXTRUDER_INVERT
#define M_PULLER_INVERT
// #define M_LEVEL_INVERT
// #define M_WINDER_INVERT

// GCODE //////////////////////////////////////////////////////////////////////

#define GCODE_QUEUE_SIZE_CHARS 32
#define GCODE_MAX_ARGS 3
#define SERIAL_BAUD_RATE 115200

// LOGGING ////////////////////////////////////////////////////////////////////
// #define ENABLE_VERBOSE_LOGGING
#define ENABLE_DEBUG_LOGGING
#define ENABLE_INFO_LOGGING
#define ENABLE_WARN_LOGGING

#define TABULAR_DEFAULT_LOG_PERIOD_MS 2000

// #define UNIT_LEVEL_TESTING // Don't use this! Run `make test` instead

#endif /* ifndef CONFIG_DEFAULTS_H */
