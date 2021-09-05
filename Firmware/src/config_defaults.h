/**
 * @Author: Nick Steele
 * @Date:   13:35 Sep 05 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 15:39 Sep 05 2021
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
#define HTR1_THERMAL_RUNAWAY 1

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

#endif /* ifndef CONFIG_DEFAULTS_H */
