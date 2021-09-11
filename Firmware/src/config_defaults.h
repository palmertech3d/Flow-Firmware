/**
 * @Author: Nick Steele
 * @Date:   13:35 Sep 05 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 16:57 Sep 11 2021
 */

#ifndef CONFIG_DEFAULTS_H
#define CONFIG_DEFAULTS_H

// MAIN HEATER /////////////////////////////////////////////////////////////////

#define PID_HTR1_P 2
#define PID_HTR1_I 5
#define PID_HTR1_D 1

// TODO: HTR max/min temps
#define HTR1_MAX_TEMP 220
#define HTR1_MIN_TEMP 0

// TODO: HTR thermal runaway
#define HTR0_TR_MAX_TIME_OUT_OF_HYST_MS 2000
#define HTR0_TR_MAX_TIME_OUT_OF_MIN_RAMP_MS 2000
#define HTR0_TR_TEMP_HYSTERESIS_C 5
#define HTR0_TR_MIN_RAMPUP_DEGC_PER_SAMPLE 1
#define HTR0_TR_RAMPUP_SAMPLE_DELAY_MS 5000

// MOTORS /////////////////////////////////////////////////////////////////////

// TODO: Motor speed units
#define M_EXTRUDER_MAX_SPEED_MILLIRAD_S 1000
#define M_PULLER_MAX_SPEED_MM_S 5
#define M_LEVEL_MAX_SPEED_MM_S 5
#define M_WINDER_MAX_SPEED_MILLIRAD_S 5

#define M_EXTRUDER_MAX_ACCELERATION_MILLIRAD_SS 500
#define M_PULLER_MAX_ACCELERATION_MM_SS 10
#define M_LEVEL_MAX_ACCELERATION_MM_SS 10
#define M_WINDER_MAX_ACCELERATION_MILLIRAD_SS 10

#define M_EXTRUDER_STEPS_PER_MILLIRAD 100
#define M_PULLER_STEPS_PER_MM_S 100
#define M_LEVEL_STEPS_PER_MM_S 100
#define M_WINDER_STEPS_PER_MILLIRAD_S 100

// GCODE //////////////////////////////////////////////////////////////////////

#define GCODE_QUEUE_SIZE_CHARS 32
#define GCODE_MAX_ARGS 3
#define SERIAL_BAUD_RATE 115200

// LOGGING ////////////////////////////////////////////////////////////////////
// #define ENABLE_VERBOSE_LOGGING
#define ENABLE_DEBUG_LOGGING
#define ENABLE_INFO_LOGGING
#define ENABLE_WARN_LOGGING

// #define UNIT_LEVEL_TESTING

#endif /* ifndef CONFIG_DEFAULTS_H */
