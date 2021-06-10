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
  }else{
<<<<<<< HEAD
    usart0_write("Error with parsing first gcode letter");
=======
>>>>>>> 156697314cddd2ad98465479b04dbcb4eca28b9e
    return 0;
  }
  if (input[2] == 0){ // if input is 2 chars long
    output[1] = '0';
    output[2] = '0';
    output[3] = input[1];
  }else if (input[3] == 0){ // If input is 3 chars long
    output[1] = '0';
    output[2] = input[1];
    output[3] = input[2];
  }else{ // If input is 4 chars or longer
    output[1] = input[1];
    output[2] = input[2];
    output[3] = input[3];
  }

<<<<<<< HEAD
=======
  output[4] = 0; // Set the string termination byte
>>>>>>> 156697314cddd2ad98465479b04dbcb4eca28b9e
  int outputInt;
  sscanf(output, "%d", &outputInt);
  return outputInt;

}
