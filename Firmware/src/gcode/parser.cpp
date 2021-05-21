#include "parser.h"
#include <stdlib.h>

int parser::parsegcode(char * input){
  char* output = (char*)malloc(sizeof(char) * 4);

  if (input[0] == 'G' || input[0] == 'g'){
    output[0] = '1';
  }else if (input [0] == 'M' || input[0] == 'm'){
    output[0] = '2';
  }else{
    return -1;
  }

  if (input[3] == 0){
    output[1] = input[1];
    output[2] = input[2];
  }else{
    output[1] = '0';
    output[2] = input[1];
    output[3] = input[3];
  }

}
