#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../HAL/megaatmega2560/serial.h"
#include "HAL/megaatmega2560/megaatmega2560.h"

parser::parser(){};

int parser::parsegcode(char* input){
  char* output = (char*)malloc(sizeof(char) * 4);

  if (input[0] == 'G' || input[0] == 'g'){
    output[0] = '1';
  }else if (input[0] == 'M' || input[0] == 'm'){
    output[0] = '2';
  }else{
    usart0_write("Not a gcode command.");
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

  output[4] = 0; // Set the string termination byte
  int outputInt;
  sscanf(output, "%d", &outputInt);
  return outputInt;

}

gcodeCommand parser::parsegcode2(char* input){
  gcodeCommand output;
  unsigned int index = 0;

  // First character: Should be an m or a g, case doesn't matter
  switch (input[index]){
    case 'G':
      output.letter = 'g';
      break;
    case 'g':
      output.letter = 'g';
      break;
    case 'M':
      output.letter = 'm';
      break;
    case 'm':
      output.letter = 'm';
      break;
    default:
      output.letter = -1;
      return output;
  }
  index++;


  // Next 1-3 characters must be the number for the command
  // If the next char is not an ASCII number, return invalid
  if (input[index] > 57 || input[index] < 48){
    output.letter = -1;
    return output;
  }

  //usart0_write_char(index);
  output.command = getIntFromString(input, index, 3);
  //usart0_write_char(index);
  _delay_ms(1);

  if (input[index] == ' '){
    index++;
  }else if (input[index] == '\0'){
    return output;
  }else{
    output.letter = -1;
    return output;
  }


  if (input[index] > 64 && input[index] < 123){ // If input[index] is a letter
    output.argChar[0] = tolower(input[index]);
  }else{
    output.letter = -1;
    return output;
  }

  index++;

  //usart0_write_char(index);
  output.argInt[0] = getIntFromString(input, index, 4);
  //usart0_write_char(index);

  return output;



}

int parser::getIntFromString(char* input, unsigned int &index, int numPlaces){
  char outputStr[numPlaces];
  outputStr[0] = input[index]; // Set the first number

  for (int i = 1; i < numPlaces; i++){
    index++;
    if (index == strlen(input) || input[index] == ' '){
      outputStr[index - 1] = '\0';
      int output = atoi(outputStr);
      return output;
    } else if (input[index] > 57 || input[index] < 48){ // If input[index] is not a number 0-9
      return -1;
    } else { // If there's another number
      outputStr[i] = input[index];
    }

  }
  index++;
  int output = atoi(outputStr);
  return output;
}

char* parser::cutString(char* input, int index){
  char output[strlen(input) - index];
  int j = 0;
  for(int i = index; i < strlen(input); i++){
    output[j] = input[i];
    j++;
  }

  return output;
}
