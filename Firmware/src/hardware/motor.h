#ifndef MOTOR_H
#define MOTOR_H
#include "../HAL/megaatmega2560/megaatmega2560.h"
#include <AccelStepper.h>

// Motor macros
#define EXTRUDER 0
#define ROLLERS 1
#define LEVEL 2
#define WINDER 3

class Motor{
public:
  // Default constructor
  Motor();

  // Starts the motor with the corresponding key
  void start(int motor_num);

  // Stops the motor with the corresponding key
  void stop(int motor_num);

  // Sets the speed of the motor with the corresponding key
  void set_speed(int motor_num, float motor_speed);

  // Gets the speed of the motor with the corresponding key
  float get_speed(int motor_num);

  // Runs the motors
  static void run();

  static AccelStepper m_extruder;
  static AccelStepper m_roller;
  static AccelStepper m_level;
  static AccelStepper m_winder;

};




#endif // MOTOR_H
