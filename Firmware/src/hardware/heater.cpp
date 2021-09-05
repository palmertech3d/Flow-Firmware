#include "heater.h"
#include <stdlib.h>
#include <Arduino.h>

// Initialize static variables for Heater class
double Heater::temp = 0;
double Heater::target_temp = 0;
double Heater::output = 0;
double Heater::Kp = 2, Heater::Ki = 5, Heater::Kd = 1; // PID constants, default vals are samples
MAX6675_Thermocouple Heater::thermometer = MAX6675_Thermocouple(PIN_THERMO_SCK, PIN_THERMO_CS, PIN_THERMO_SO);
PID Heater::temp_controller = PID(&Heater::temp, &Heater::output, &Heater::target_temp, Heater::Kp, Heater::Ki, Heater::Kd, DIRECT);
PID_ATune Heater::pid_auto = PID_ATune(&Heater::temp, &Heater::output);
bool Heater::auto_on = false;

// Default constructor
Heater::Heater(){
}

// Initializes the heater with a target temp
void Heater::init(double input_temp){
  target_temp = input_temp;
  temp = thermometer.readCelsius();
  temp_controller.SetMode(AUTOMATIC);
} // Heater::init

// Sets the temperature of the heater
void Heater::set(double input_temp){
  target_temp = input_temp;
} // Heater::set

// Gets the current temperature of the heater
double Heater::get(){
  return temp;
} // Heater::get

// Gets the target temperature of the heater
double Heater::get_target(){
  return target_temp;
} // Heater::get_target

// Sets the PID constants for the heater
void Heater::set_constants(double Kp_set, double Ki_set, double Kd_set){
  Kp = Kp_set;
  Ki = Ki_set;
  Kd = Kd_set;
  temp_controller.SetTunings(Kp, Ki, Kd);
} // Heater::set_constants

void Heater::get_constants(double *constants_out){
  constants_out[0] = Kp;
  constants_out[1] = Ki;
  constants_out[2] = Kd;
} // Heater::get_constants

void Heater::autotune_init(){
  auto_on = true;
  pid_auto.SetControlType(1); // Set to PID control type
  pid_auto.SetNoiseBand(2); // Set to ignore 2 degrees of noise
} // Heater::autotune_init

bool Heater::autotune_on(){
  return auto_on;
} // Heater::autotune_on

void Heater::update(){
  double temporary = thermometer.readCelsius();

  if ((int)temporary != (int)temp) {
    Serial.print(F("Hotend: ")); Serial.print((int)temp); Serial.println(F("C"));
  }

  temp = temporary;

  temp_controller.Compute();
  analogWrite(PIN_HEATER, output);

  if (auto_on) {
    int tune_result = pid_auto.Runtime();
    if (tune_result != 0) {
      auto_on = false;

      // return PID constants
      set_constants(pid_auto.GetKd(), pid_auto.GetKi(), pid_auto.GetKd());
      Serial.println(F("Autotuning complete. Constants stored in heater."));
      Serial.println(F("Use m503 to view constants."));
    }
  }
} // Heater::update
