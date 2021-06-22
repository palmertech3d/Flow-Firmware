#include "gcode.h"

#include <Arduino.h>
#include <stdlib.h>
#include "../HAL/megaatmega2560/serial.h"

gcode::gcode(){};

int gcode::get_gcode(){

  // first, poll serial for inputs
  parser parserHandler;
  char* gcodeInChar = (char *)malloc(sizeof(char) * 20); // 20 corresponds to the
                                                         // max # of chars a
                                                         // gcode command can be
  int i = 0;
  if (Serial.available()){
    char temp = Serial.read();
    while(Serial.available() && temp != 13){ // 13 is the "enter" key
        gcodeInChar[i] = temp;
        //usart0_write_char(gcodeInChar[i]); // Feedback for user
        temp = Serial.read();
        i++;
      }
  }

  if (i == 0){
    //Serial.print("Returned 1");
    return 1;
  }

  gcodeInChar[i] = 0; // Terminate the string
  //usart0_write_str("\r\nYour input:\r\n");
  //usart0_write_str(gcodeInChar);
  //usart0_write_str("\r\n");

  // parse this input
  gcodeCommand parsedGcode = parserHandler.parsegcode(gcodeInChar);

  // verify the parsed gcode
  if (parsedGcode.letter == -1){
    usart0_write_str("Invalid gcode entered.");
    return 1; // return 1 to indicate invalid gcode entered
  }

  /*
  // display details about the parsed gcode
  usart0_write_str("Your gcode command:\r\n");
  usart0_write_char(parsedGcode.letter);
  usart0_write_int(parsedGcode.command);
  usart0_write_str("\r\n");
  usart0_write_char(parsedGcode.argChar[0]);
  usart0_write_int(parsedGcode.argInt[0]);
  usart0_write_str("\r\n");
  usart0_write_char(parsedGcode.argChar[1]);
  usart0_write_int(parsedGcode.argInt[1]);
  */

  Serial.println("OK");

  // put the parsed gcode in the buffer
  buffer.putForce(parsedGcode);

  return 0;
}

bool gcode::execute_gcode(gcodeCommand command){

  if (command.letter == 'g' && command.command == 28){
    g28();
    return 1;
  }else if (command.letter == 'g' && command.command == 1 && command.argChar[0] == 'm' && command.argChar[1] == 's'){
    g1(command.argInt[0], command.argInt[1]);
    return 1;
  }else if (command.letter == 'g' && command.command == 2 && command.argChar[0] == 'm'){
    g2(command.argInt[0]);
    return 1;
  }else if (command.letter == 'm' && command.command == 104 && command.argChar[0] == 'b'){
    m104(command.argInt[0]);
    return 1;
  }else if (command.letter == 'm' && command.command == 104){
    m104();
    return 1;
  }else if (command.letter == 'm' && command.command == 106){
    m106();
    return 1;
  }else if (command.letter == 'm' && command.command == 303){
    m303();
    return 1;
  }else if (command.letter == 'm' && command.command == 503){
    m503();
    return 1;
  }

  else{
    return 0; // return false to indicate that no gcode was executed
  }

  return 1;
}

bool gcode::execute_buffer(){
  return execute_gcode(buffer.get());
}
