#include "gcode.h"
#include "config_defaults.h"

#include <Arduino.h>
#include <stdlib.h>
#include "../HAL/megaatmega2560/serial.h"

gcode::gcode(){
};

GcodeError_t gcode::getGcode() {
  // first, poll serial for inputs
  parser parserHandler;
  char gcodeInChar[GCODE_QUEUE_SIZE_CHARS];

  uint8_t i = 0;
  if (Serial.available()) {
    char temp = Serial.read();
    while (Serial.available() && temp != '\n' && temp != '\r' && i < GCODE_QUEUE_SIZE_CHARS) {
      gcodeInChar[i] = temp;
      temp = Serial.read();
      i++;
    }
  }

  if (i == 0) return ERR_NO_DATA;
  if (i >= GCODE_QUEUE_SIZE_CHARS) return ERR_TOO_LONG;

  gcodeInChar[i] = '\0'; // Terminate the string

  // parse this input
  GcodeCommand_t parsedGcode = parserHandler.parsegcode(gcodeInChar);

  // verify the parsed gcode
  if (parsedGcode.letter == GCODE_LETTER_ERR) {
    Serial.println(F("Invalid gcode entered."));
    return ERR_GCODE_INVALID; // return 1 to indicate invalid gcode entered
  }

  Serial.println(F("OK"));

  // put the parsed gcode in the buffer
  buffer.putForce(parsedGcode);

  return ERR_SUCCESS;
} // gcode::getGcode

GcodeError_t gcode::executeGcode(GcodeCommand_t cmd) {
  if (cmd.letter == 'g') {
    switch (cmd.command) {
    case 1: g1(cmd); break;
    case 2: g2(cmd); break;
    case 28: g28(cmd); break;
    default: return ERR_NO_SUCH_NUMBER;
    } // switch
  } else if (cmd.letter == 'm') {
    switch (cmd.command) {
    case 104: m104(cmd); break;
    case 106: m106(cmd); break;
    case 303: m303(cmd); break;
    case 503: m503(cmd); break;
    default: return ERR_NO_SUCH_NUMBER;
    } // switch
  } else {
    return ERR_NO_SUCH_LETTER;
  }

  return ERR_SUCCESS;
} // gcode::executeGcode

GcodeError_t gcode::executeBuffer(){
  return executeGcode(buffer.get());
} // gcode::executeBuffer

GcodeArg_t gcodeParseValueFor(char letter, GcodeCommand_t cmd) {
  for (uint8_t i = 0; i < GCODE_MAX_ARGS; i++) {
    if (cmd.arg_char[i] == letter) {
      return cmd.arg_value[i];
    }
  }
  return GCODE_ARG_VALUE_ERR;
} // getCurrentArgument
