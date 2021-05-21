#include "gcode.h"
#include "../HAL/megaatmega2560/serial.h"
#include "parser.h"

gcode::gcode(){};

int gcode::get_gcode(){

  // first, poll serial for inputs
  parser* parserHandler = new parser();
  char* gcodeInChar = 0;
  int i = 0;

  while (usart0_receive() != 13){
    gcodeInChar[i] = usart0_receive();
    i++;
  }


  // TODO: then parse this input
  int parsedgcode = parserHandler->parsegcode(gcodeInChar);

  // TODO: then put the parsed gcode in the buffer
  usart0_write_int(parsedgcode);

  return 1;
}

int execute_buffer(){
  // TODO: first find the first in gcode in the buffer
  // TODO: then execute this gcode
  // TODO: check if gcode executed successfully
  return 1;
}
