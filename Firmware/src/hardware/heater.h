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
  void init(double input_temp);

  // Sets the temperature of the heater
  void set(double input_temp);

  // Gets the current temperature of the heater
  double get();

  // Gets the target temperature of the heater
  double get_target();

  // Sets the PID constants for the heater
  void set_constants(double Kp_set, double Ki_set, double Kd_set);

  // Gets the PID constants for the heater and stores them in a double* passed
  // by reference
  void get_constants(double* constants_out);

  // Experimental: Automatically tunes the PID constants for the heater
  void autotune_init();

  // Updates the heater from the calculated PID value
  // Must be called regularly
  void update();

private:
  double temp; // The current temperature of the hotend
  double target_temp; // The target temperature of the hotend
  double output; // The value calculated by the PID to write to the heater
  double Kp=2, Ki=5, Kd=1; // PID constants, default vals are samples
  MAX6675_Thermocouple thermometer = MAX6675_Thermocouple(THERMO_SCK, THERMO_CS, THERMO_SO);
  PID temp_controller = PID(&temp, &output, &target_temp, Kp, Ki, Kd, DIRECT);

  // Autotune
  PID_ATune pid_auto = PID_ATune(&temp, &output);
  bool auto_on = false;

};



#endif // HEATER_H
