#include "heater.h"
#include <stdlib.h>

// Default constructor
Heater::Heater(){}

// Initializes the heater with a target temp
void Heater::init(double input_temp){
  target_temp = input_temp;
  temp = thermometer.readCelsius();
  temp_controller.SetMode(AUTOMATIC);
}

// Sets the temperature of the heater
void Heater::set(double input_temp){
  target_temp = input_temp;
}

// Gets the current temperature of the heater
double Heater::get(){
  return temp;
}

// Gets the target temperature of the heater
double Heater::get_target(){
  return target_temp;
}

// Sets the PID constants for the heater
void Heater::set_constants(double Kp_set, double Ki_set, double Kd_set){
  Kp = Kp_set;
  Ki = Ki_set;
  Kd = Kd_set;
}

void Heater::update_heater(){
  temp = thermometer.readCelsius();

  char* tempChar;
  itoa((uint8_t)temp, tempChar, 10);


  usart0_write_str(tempChar);
  temp_controller.Compute();
  usart0_write_str("\r\n");
  analogWrite(HEATER, output);
}
