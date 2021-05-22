#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include "../HAL/megaatmega2560/serial.h"

parser::parser(){};
int parser::parsegcode(char* input){
  char* output = (char*)malloc(sizeof(char) * 4);

  if (input[0] == 'G' || input[0] == 'g'){
    output[0] = '1';
  }else if (input[0] == 'M' || input[0] == 'm'){
    output[0] = '2';
    usart0_write("Selected an M");
  }else{
    usart0_write("This failed.");
    return 0;
  }

  if (input[3] == 0){ // If input is only 3 chars long
    output[1] = '0';
    output[2] = input[1];
    output[3] = input[2];
    usart0_write("Input is three chars long.");
  }else{ // If input is 4 chars or longer
    output[1] = input[1];
    output[2] = input[2];
    output[3] = input[3];
    usart0_write("Input is four or more chars long.\r\n");
  }

  //usart0_write(output);
  int outputInt;
  sscanf(output, "%d", &outputInt);
  usart0_write("\r\nNot stuck!");
  return outputInt;

}
