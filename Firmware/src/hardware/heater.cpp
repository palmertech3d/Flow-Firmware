#include "heater.h"
#include <stdlib.h>
#include <Arduino.h>

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

void Heater::get_constants(double* constants_out){
  constants_out[0] = Kp;
  constants_out[1] = Ki;
  constants_out[2] = Kd;
}

void Heater::autotune_init(){
  auto_on = true;
  pid_auto.SetControlType(1); // Set to PID control type
  pid_auto.SetNoiseBand(2); // Set to ignore 2 degrees of noise

}

void Heater::update_heater(){
  temp = thermometer.readCelsius();

  // Writing out current temperature to USART
  char* tempChar;
  itoa((uint8_t)temp, tempChar, 10); // 10 is for base 10
  usart0_write_str(tempChar);
  usart0_write_str("\r\n");

  temp_controller.Compute();
  analogWrite(HEATER, output);

  int tune_result = 0;

  if(auto_on){
    tune_result = pid_auto.Runtime();
    if (tune_result != 0){
      auto_on = false;
      // return PID constants
      Kp = pid_auto.GetKp();
      Ki = pid_auto.GetKi();
      Kd = pid_auto.GetKd();
      usart0_write_str("PID Autotuning complete. Constants stored in heater.");

      Serial.begin(9600);
      Serial.print("Kp: ");
      Serial.print(Kp);
      Serial.print("Ki: ");
      Serial.print(Ki);
      Serial.print("Kd: ");
      Serial.print(Kd);
    }
  }
}
