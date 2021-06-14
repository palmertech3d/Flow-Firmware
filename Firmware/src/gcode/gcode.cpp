#include "gcode.h"

#include <stdlib.h>
#include "../HAL/megaatmega2560/serial.h"
#include "parser.h"

gcode::gcode(){};

int gcode::get_gcode(){

  // first, poll serial for inputs
  parser parserHandler;
  char* gcodeInChar = (char *)malloc(sizeof(char) * 4); // 4 corresponds to the
                                                        // max # of chars a
                                                        // gcode command can be

  char temp = usart0_receive();
  int i = 0;
  while(temp != 13){

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
  int parsedgcode = parserHandler.parsegcode(gcodeInChar);

  // TODO: then put the parsed gcode in the buffer

  //usart0_write_int(parsedgcode);

  return 0;
}

int execute_buffer(){
  // TODO: first find the first in gcode in the buffer
  // TODO: then execute this gcode
  // TODO: check if gcode executed successfully
  return 1;
}
