#include "parser.h"
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../HAL/megaatmega2560/serial.h"
#include "HAL/megaatmega2560/megaatmega2560.h"

parser::parser(){
};

gcodeCommand parser::parsegcode(char *input){
  gcodeCommand output; // The gcodeCommand object to return
  unsigned int index = 0; // The index used to iterate through the user's input

  // Initialize output with default values
  output.letter = 0;
  output.command = 0;
  output.argChar[0] = 0;
  output.argInt[0] = 0;
  output.argChar[1] = 0;
  output.argInt[1] = 0;
  output.argChar[2] = 0;
  output.argInt[2] = 0;

  // First character: Should be an m or a g, case doesn't matter
  switch (input[index]) {
  case 'G':
  case 'g':
    output.letter = 'g';
    break;
  case 'M':
  case 'm':
    output.letter = 'm';
    break;
  default:
    output.letter = -1;
    return output;
  } // switch

  index++; // Move to the second character in input

  // Get the command number that should occur directly after the letter
  char subString[strlen(input) - index]; // Create a substring
  cutString(input, subString, &index); // Fill the substring
  int temp = getIntFromString(subString, 3);
  output.command = temp; // Get the data from the substring

  // If an invalid form was found in getIntFromString, return invalid
  if (output.command == -1) {
    output.letter = -1;
    return output;
  }

  // Check for a space or a null byte.
  // If a space, move to get arguments.
  // If a null byte, return output.
  // If anything else, return invalid
  if (input[index] == ' ') {
    index++;
  } else if (input[index] == '\0') {
    return output;
  } else {
    output.letter = -1; // Return invalid
    return output;
  }

  // Move to get command parameters
  for (int i = 0; i < 2; i++) {
    if ((input[index] >= 'A' && input[index] <= 'Z') || (input[index] >= 'a' && input[index] <= 'z')) { // If input[index] is a letter
      output.argChar[i] = tolower(input[index]);
    } else {
      output.letter = -1; // Return invalid
      return output;
    }

    index++;

    cutString(input, subString, &index); // Fill the substring
    int temp2 = getIntFromString(subString, 4);

    // If an invalid form was found in getIntFromString, return invalid
    if (output.argInt[i] == -1) {
      output.letter = -1;
      return output;
    }
    output.argInt[i] = temp2; // Get the data from the substring

    // Check for a space or a null byte.
    // If a space, move to get arguments.
    // If a null byte, return output.
    // If anything else, return invalid
    if (input[index] == ' ') {
      index++;
    } else if (input[index] == '\0') {
      return output;
    } else {
      output.letter = -1; // Return invalid
      return output;
    }
  }
  return output;
} // parser::parsegcode

// TODO: Replace this function with a simple implementation of atoi()
int parser::getIntFromString(char *input, int numPlaces){
  char outputStr[numPlaces + 1];
  int j = 0;
  for (int i = 0; i < numPlaces; i++) {
    if (isNum(input[i])) {
      outputStr[j] = input[i];
      j++;
    } else if (input[i] == ' ' || input[i] == '\0') {
      i = numPlaces;
    } else {
      return -1;
    }
  }
  outputStr[j] = '\0';
  int output = atoi(outputStr);
  return output;
} // parser::getIntFromString

void parser::cutString(char *input, char *output, unsigned int *index){
  int j = 0;

  while (input[*index] != ' ' && input[*index] != '\0') {
    output[j] = input[*index];
    j++;
    ++*index;
  }
  output[j] = '\0';
} // parser::cutString

bool parser::isNum(char data){
  return  (data >= '0' && data <= '9');
} // parser::isNum
