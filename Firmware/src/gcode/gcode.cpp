#include "gcode.h"

#include <stdlib.h>
#include "../HAL/megaatmega2560/serial.h"
//#include "parser.h"

gcode::gcode(){};

int gcode::get_gcode(){

  // first, poll serial for inputs
  parser parserHandler;
  char* gcodeInChar = (char *)malloc(sizeof(char) * 20); // 20 corresponds to the
                                                         // max # of chars a
                                                         // gcode command can be

  char temp = usart0_receive();
  int i = 0;
  while(temp != 13){ // 13 is the "enter" key
    gcodeInChar[i] = temp;
    usart0_write_char(gcodeInChar[i]); // Feedback for user
    temp = usart0_receive();
    i++;
  }

  gcodeInChar[i] = 0;
  usart0_write_str("\r\nYour input:\r\n");
  usart0_write_str(gcodeInChar);
  usart0_write_str("\r\n");

  // parse this input
  gcodeCommand parsedGcode = parserHandler.parsegcode(gcodeInChar);

  // verify the parsed gcode
  if (parsedGcode.letter == -1){
    usart0_write_str("Invalid gcode entered.");
    return 1; // return 1 to indicate invalid gcode entered
  }

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

  // put the parsed gcode in the buffer
  buffer.put(parsedGcode);

  return 0;
}

bool gcode::execute_gcode(gcodeCommand command){
  if (command.letter == 'g' && command.command == 28){
    G28();
    return 1;
  }else if (command.letter == 'g' && command.command == 1 && command.argChar[0] == 'm' && command.argChar[1] == 's'){
    // execute g1(argInt[0], argInt[1]) to turn on motor argInt[0] with speed argInt[1]
  }else if (command.letter == 'g' && command.command == 2 && command.argChar[0] == 'm'){
    // execute g2(argInt[0]) to turn off motor argInt[0]
  }else if (command.letter == 'm' && command.command == 104 && command.argChar[0] == 'b'){
    // execute m104(argInt[0]) to turn on hotend with temp argInt[0]
  }else if (command.letter == 'm' && command.command == 104){
    // execute m104() to turn off hotend
  }else if (command.letter == 'm' && command.command == 106 && command.argChar[0] == 's'){
    // execute m106(argInt[0]) to turn on the cooling fans with speed argInt[0]
  }else if (command.letter == 'm' && command.command == 106){
    // execute m106() to turn off the cooling fans
  }

  else{
    return 0; // return false to indicate that no gcode was executed
  }
}

bool gcode::execute_buffer(){
  return execute_gcode(buffer.get());
}
