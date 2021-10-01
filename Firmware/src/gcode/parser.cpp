#include "parser.h"
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../HAL/megaatmega2560/serial.h"
#include "HAL/megaatmega2560/megaatmega2560.h"

GcodeParser::GcodeParser(){
};

GcodeCommand_t GcodeParser::parsegcode(char *input){
  GcodeCommand_t output; // The GcodeCommand_t object to return
  unsigned int index = 0; // The index used to iterate through the user's input

  // Initialize output with default values
  output.letter = 0;
  output.command = 0;
  output.arg_char[0] = 0;
  output.arg_value[0] = 0;
  output.arg_char[1] = 0;
  output.arg_value[1] = 0;
  output.arg_char[2] = 0;
  output.arg_value[2] = 0;

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
    output.letter = GCODE_LETTER_ERR;
    return output;
  } // switch

  index++; // Move to the second character in input

  // Get the command number that should occur directly after the letter
  char sub_string[strlen(input) - index]; // Create a substring
  cutString(input, sub_string, &index); // Fill the substring
  int temp = getIntFromString(sub_string, 3);
  output.command = temp; // Get the data from the substring

  // If an invalid form was found in getIntFromString, return invalid
  if (output.command == GCODE_COMMAND_ERR) {
    output.letter = GCODE_LETTER_ERR;
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
    output.letter = GCODE_LETTER_ERR; // Return invalid
    return output;
  }

  // Move to get command parameters
  for (int i = 0; i < GCODE_MAX_ARGS; i++) {
    if ((input[index] >= 'A' && input[index] <= 'Z') || (input[index] >= 'a' && input[index] <= 'z')) { // If input[index] is a letter
      output.arg_char[i] = tolower(input[index]);
    } else {
      output.letter = GCODE_LETTER_ERR; // Return invalid
      return output;
    }

    index++;

    cutString(input, sub_string, &index); // Fill the substring
    // GcodeArg_t arg_from_string = getIntFromString(sub_string, 4);
    GcodeArg_t arg_from_string = (GcodeArg_t)atof(sub_string);

    // If an invalid form was found in getIntFromString, return invalid
    if (arg_from_string == -1) {
      output.letter = GCODE_ARG_CHAR_ERR;
    } else {
      output.arg_value[i] = arg_from_string; // Get the data from the substring
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
      output.letter = GCODE_LETTER_ERR; // Return invalid
      return output;
    }
  }
  return output;
} // GcodeParser::parsegcode

// TODO: Replace this function with a simple implementation of atoi()
int GcodeParser::getIntFromString(char *input, int numPlaces){
  char outputStr[numPlaces + 1];
  int j = 0;
  for (int i = 0; i < numPlaces; i++) {
    if (isNum(input[i])) {
      outputStr[j] = input[i];
      j++;
    } else if (input[i] == ' ' || input[i] == '\0') {
      i = numPlaces;
    } else {
      return GCODE_COMMAND_ERR;
    }
  }
  outputStr[j] = '\0';
  int output = atoi(outputStr);
  return output;
} // GcodeParser::getIntFromString

// int GcodeParser::getIntFromString(char *input, int numPlaces) {
//   return 0;
// } // GcodeParser::getIntFromString

void GcodeParser::cutString(char *input, char *output, unsigned int *index){
  int j = 0;

  while (input[*index] != ' ' && input[*index] != '\0') {
    output[j] = input[*index];
    j++;
    ++*index;
  }
  output[j] = '\0';
} // GcodeParser::cutString

bool GcodeParser::isNum(char data){
  return  (data >= '0' && data <= '9');
} // GcodeParser::isNum

// TEST FUNCTIONS /////////////////////////////////////////////////////////////

#ifdef UNIT_LEVEL_TESTING

void testParsing(GcodeCommand_t *correct_values, GcodeCommand_t *test_values, TestResult_t *accumulator) {
  TestResult_t subaccumulator;
  TEST_ASSERT_EQUAL(test_values->letter, correct_values->letter, subaccumulator);
  TEST_ASSERT_EQUAL(test_values->command, correct_values->command, subaccumulator);
  TEST_ASSERT_EQUAL(test_values->arg_char[0], correct_values->arg_char[0], subaccumulator);
  TEST_ASSERT_EQUAL(test_values->arg_char[1], correct_values->arg_char[1], subaccumulator);
  TEST_ASSERT_EQUAL(test_values->arg_char[2], correct_values->arg_char[2], subaccumulator);
  TEST_ASSERT_EQUAL(test_values->arg_value[0], correct_values->arg_value[0], subaccumulator);
  TEST_ASSERT_EQUAL(test_values->arg_value[1], correct_values->arg_value[1], subaccumulator);
  TEST_ASSERT_EQUAL(test_values->arg_value[2], correct_values->arg_value[2], subaccumulator);
  accumulator->total += subaccumulator.total;
  accumulator->failed += subaccumulator.failed;
} // testParsing

TestResult_t GcodeParser::TEST_gcodeParser() {
  TestResult_t accumulator;
  GcodeCommand_t correct_parsing;
  static char cmd[GCODE_QUEUE_SIZE_CHARS] = {"G1 A0 B1.1 C-1000"};
  correct_parsing.letter = 'g';
  correct_parsing.command = 1;
  correct_parsing.arg_char[0] = 'a';
  correct_parsing.arg_value[0] = 0;
  correct_parsing.arg_char[1] = 'b';
  correct_parsing.arg_value[1] = 1.1;
  correct_parsing.arg_char[2] = 'c';
  correct_parsing.arg_value[2] = -1000;
  GcodeCommand_t parsed = this->parsegcode(cmd);
  testParsing(&correct_parsing, &parsed, &accumulator);
  return accumulator;
} // TEST_gcode
#endif // ifdef UNIT_LEVEL_TESTING
