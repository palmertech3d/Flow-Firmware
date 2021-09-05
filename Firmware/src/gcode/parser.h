#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "config_defaults.h"

typedef int16_t GcodeArg_t;
#define GCODE_ARG_VALUE_ERR -1
#define GCODE_ARG_CHAR_ERR -1
#define GCODE_LETTER_ERR -1
#define GCODE_COMMAND_ERR -1

// A struct to hold a gcode command
struct GcodeCommand_t {
  char letter; // G or an M
  uint16_t command; // Number of the gcode command
  char arg_char[GCODE_MAX_ARGS]; // Letters for arguments
  GcodeArg_t arg_value[GCODE_MAX_ARGS]; // Values for arguments
};

class GcodeParser {
public:

// Default constructor
GcodeParser();

// Takes in a char* for an inputted string.
// Returns a GcodeCommand_t object corresponding
// to the gcode command parsed.
// Returns a GcodeCommand_t object with property
// char letter = GCODE_LETTER_ERR if inputed string is invalid.
GcodeCommand_t parsegcode(char *);

private:

// Cuts a string at the passed index, returning the string from the index
// to a space or a null byte
void cutString(char *input, char *output, unsigned int *index);

// TODO: Replace this function with a simple implementation of atoi()
int getIntFromString(char *input, int numPlaces);

// Takes in a char, returns true if it is a num, false if not.
bool isNum(char data);
}; // class GcodeParser

#endif // PARSER_H
