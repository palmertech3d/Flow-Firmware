#ifndef HEATER_H
#define HEATER_H
#include "../HAL/megaatmega2560/megaatmega2560.h"
#include "../HAL/megaatmega2560/serial.h"
#include <PID_v1.h> // For PID control
#include <PID_AutoTune_v0.h>
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>

class Heater{
public:
  // Default constructor
  Heater();

  // Initializes the heater with a target temp
  static void init(double input_temp);

  // Sets the temperature of the heater
  static void set(double input_temp);

  // Gets the current temperature of the heater
  static double get();

  // Gets the target temperature of the heater
  static double get_target();

  // Sets the PID constants for the heater
  static void set_constants(double Kp_set, double Ki_set, double Kd_set);

  // Gets the PID constants for the heater and stores them in a double* passed
  // by reference
  static void get_constants(double* constants_out);

  // Experimental: Automatically tunes the PID constants for the heater
  static void autotune_init();

  // Returns true if autotune is active, false if not
  static bool autotune_on();

  // Updates the heater from the calculated PID value
  // Must be called regularly
  static void update();

private:
  static double temp; // The current temperature of the hotend
  static double target_temp; // The target temperature of the hotend
  static double output; // The value calculated by the PID to write to the heater
  static double Kp,Ki,Kd;
  static MAX6675_Thermocouple thermometer;
  static PID temp_controller;

  // Autotune
  static PID_ATune pid_auto;
  static bool auto_on;

};



#endif // HEATER_H
