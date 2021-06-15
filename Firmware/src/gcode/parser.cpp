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
  gcodeCommand output; // The gcodeCommand object to return
  unsigned int index = 0; // The index used to iterate through the user's input

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

  index++; // Move to the second character in input


  // Get the command number that should occur directly after the letter
  char subString[strlen(input) - index]; // Create a substring
  cutString(input, subString, &index); // Fill the substring
  int temp = getIntFromString(subString, 3);
  output.command = temp; // Get the data from the substring


  // If an invalid form was found in getIntFromString, return invalid
  if(output.command == -1){
    output.letter = -1;
    return output;
  }

  // Check for a space or a null byte.
  // If a space, move to get arguments.
  // If a null byte, return output.
  // If anything else, return invalid
  if (input[index] == ' '){
    index++;
  }else if (input[index] == '\0'){
    return output;
  }else{
    output.letter = -1; // Return invalid
    return output;
  }

  if (input[index] > 64 && input[index] < 123){ // If input[index] is a letter
    output.argChar[0] = tolower(input[index]);
  }else{
    output.letter = -1; // Return invalid
    return output;
  }

  index++;

  cutString(input, subString, &index); // Fill the substring
  int temp2 = getIntFromString(subString, 4);

  // If an invalid form was found in getIntFromString, return invalid
  if(output.argInt[0] == -1){
    output.letter = -1;
    return output;
  }

  output.argInt[0] = temp2; // Get the data from the substring
  return output;

}

int parser::getIntFromString(char* input, int numPlaces){
  char outputStr[numPlaces + 1];
  int j = 0;
  for (int i = 0; i < numPlaces; i++){
    if (isNum(input[i])){
      outputStr[j] = input[i];
      j++;
    }else if (input[i] == ' ' || input[i] == '\0'){
      i = numPlaces;
    }else{
      return -1;
    }
  }

  outputStr[j] = '\0';
  int output = atoi(outputStr);
  return output;
}



void parser::cutString(char* input, char* output, int* index){
  int j = 0;

  while(input[*index] != ' ' && input[*index] != '\0'){
    output[j] = input[*index];
    j++;
    ++*index;
  }
  output[j] = '\0';
}

bool parser::isNum(char data){
  if(data > 47 && data < 58){return true;}
  else{return false;}
}
