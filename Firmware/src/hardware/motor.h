#ifndef MOTOR_H
#define MOTOR_H

#include "../HAL/megaatmega2560/megaatmega2560.h"
#include "blackboard/blackboard.h"
#include "logger.h"
#include "test/unit-testing.h"
#include <AccelStepper.h>
#include "tabular/tabular.h"

// Motor macros
#define EXTRUDER 0
#define PULLERS 1
#define LEVEL 2
#define WINDER 3

class Motor {
public:

// Default constructor
static void init();

// Starts the motor with the corresponding key
static void start(int motor_num);

static bool isValidMotor(int16_t motor) {
  return (motor == EXTRUDER || motor == PULLERS || motor == WINDER || motor == LEVEL);
} // isValidMotor

// Stops the motor with the corresponding key
static void stop(int motor_num);

// Sets the speed of the motor with the corresponding key
static void setSpeed(int motor_num, float motor_speed);

// Gets the speed of the motor with the corresponding key
static float getSpeed(int motor_num);

// Runs the motors
static void run();

// Homes the level winder
static void homeLevelWinder();

/** Runs any safety tests needed, ex. minimum extrusion temperatures.
 * Must be run periodically.
 */
static void idle();

// Set the bounds for the winder
static void set_winder_bounds(int left, int right);

/**
 * Callback for the tabular data to access the motor data
 *
 * @param  col [description]
 * @return     [description]
 */
static TabularCallback_t motorTabularCallback(uint8_t col);

static void enableReporter(uint16_t rate);

static void disableReporter();

static AccelStepper m_extruder;
static AccelStepper m_roller;
static AccelStepper m_level;
static AccelStepper m_winder;

#ifdef UNIT_LEVEL_TESTING

static TestResult_t TEST_preventColdExtrusion();
#endif // ifdef UNIT_LEVEL_TESTING

private:

static float realUnitsToMotorTicks(int motor_num, float speed);

static float motorTicksToRealUnits(int motor_num, float tick_rate);

static long winder_bound_left;
static long winder_bound_right;
static bool level_homed;
static bool level_running;

// For data logging
static Tabular_t tabular_obj;
}

;

#endif // MOTOR_H
