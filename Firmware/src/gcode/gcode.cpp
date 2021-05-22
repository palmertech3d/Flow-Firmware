#include "gcode.h"

#include <stdlib.h>
#include "../HAL/megaatmega2560/serial.h"
#include "parser.h"

gcode::gcode(){};

int gcode::get_gcode(){

  // first, poll serial for inputs
  parser* parserHandler = new parser();
  char* gcodeInChar = (char *)malloc(sizeof(char) * 4);

  char temp = usart0_receive();
  int i = 0;
  while(temp != 13){

    gcodeInChar[i] = temp;

    usart0_write("Echo: ");
    for (int j = 0; j <= i; j++){
      usart0_write_int(gcodeInChar[j]);

    }
    usart0_write("\r\n");
    temp = usart0_receive();
    i++;
  }

  gcodeInChar[i] = 0;

  // TODO: then parse this input
  uint8_t parsedgcode = parserHandler->parsegcode(gcodeInChar);

  // TODO: then put the parsed gcode in the buffer
  //usart0_write_int(111);
  usart0_write_int(parsedgcode);

  return 0;
}

int execute_buffer(){
  // TODO: first find the first in gcode in the buffer
  // TODO: then execute this gcode
  // TODO: check if gcode executed successfully
  return 1;
}
