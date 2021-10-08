#include "gcode.h"
#include "config_defaults.h"

#include <Arduino.h>
#include <stdlib.h>
#include "../HAL/megaatmega2560/serial.h"

GcodeExecuter::GcodeExecuter(){
};

static char gcodeInChar[GCODE_QUEUE_SIZE_CHARS];
static uint8_t gcode_buffer_idx = 0;

GcodeError_t GcodeExecuter::getGcode() {
  // first, poll serial for inputs
  GcodeParser parser_handler;
  char recv = -1;
  if (Serial.available()) {
    // Serial.println("available");
    delay(1);
    recv = Serial.read();
    while (Serial.available() && recv != '\n' && recv != '\r' && gcode_buffer_idx < GCODE_QUEUE_SIZE_CHARS) {
      gcodeInChar[gcode_buffer_idx] = recv;
      recv = Serial.read();
      gcode_buffer_idx++;
    }
  }

  if (gcode_buffer_idx == 0) return GCODE_ERR_NO_DATA;
  if (gcode_buffer_idx >= GCODE_QUEUE_SIZE_CHARS) return GCODE_ERR_TOO_LONG;

  if (recv != '\n' && recv != '\r') {
    // Line did not end; return doing nothing, hoping that more data will
    // soon be available
    return GCODE_ERR_SUCCESS;   // Pretend it succeeded for now
  }
  // If we got here, the line ended without any errors
  gcodeInChar[gcode_buffer_idx] = '\0'; // Terminate the string
  gcode_buffer_idx = 0; // Reset the index for the next loop since everything completed

  Serial.println(gcodeInChar);

  // parse this input
  GcodeCommand_t parsedGcode = parser_handler.parsegcode(gcodeInChar);

  gcodeInChar[0] = 0; // Finish resetting inputs

  // verify the parsed gcode
  if (parsedGcode.letter == GCODE_LETTER_ERR || parsedGcode.command == GCODE_COMMAND_ERR) {
    LOG_WARN("Invalid gcode entered.\n");
    LOG_DEBUG(F("GCODE Read from buffer: ")); LOG_DEBUG(gcodeInChar); LOG_DEBUG('\n');
    return GCODE_ERR_GCODE_INVALID; // return 1 to indicate invalid gcode entered
  }

  LOG_INFO(F("ok\n"));

  // put the parsed gcode in the buffer
  buffer.putForce(parsedGcode);

  return GCODE_ERR_SUCCESS;
} // GcodeExecuter::getGcode

GcodeError_t GcodeExecuter::tryToAddGcodeToBuffer(GcodeCommand_t gcode) {
  if (gcode.letter == GCODE_LETTER_ERR || gcode.command == GCODE_COMMAND_ERR) {
    LOG_ERROR("Failed to add gcode to buffer from hardcoded gcode.");
    return GCODE_ERR_GCODE_INVALID;
  }
  buffer.putForce(gcode);
  return GCODE_ERR_SUCCESS;
} // tryToAddGcodeToBuffer

GcodeError_t GcodeExecuter::executeGcode(GcodeCommand_t cmd) {
  if (cmd.letter == 'g') {
    switch (cmd.command) {
    case 1: g1(cmd); break;
    case 2: g2(cmd); break;
    case 28: g28(cmd); break;
    default: return GCODE_ERR_NO_SUCH_NUMBER;
    } // switch
  } else if (cmd.letter == 'm') {
    switch (cmd.command) {
    case 104: m104(cmd); break;
    case 106: m106(cmd); break;
    case 156: m156(cmd); break;
    case 303: m303(cmd); break;
    case 503: m503(cmd); break;
    default: return GCODE_ERR_NO_SUCH_NUMBER;
    } // switch
  } else {
    return GCODE_ERR_NO_SUCH_LETTER;
  }

  return GCODE_ERR_SUCCESS;
} // GcodeExecuter::executeGcode

GcodeError_t GcodeExecuter::executeBuffer(){
  return executeGcode(buffer.get());
} // GcodeExecuter::executeBuffer

// Functions for use without GCODE handler ////////////////////////////////////

GcodeArg_t gcodeParseValueFor(char letter, GcodeCommand_t cmd) {
  for (uint8_t i = 0; i < GCODE_MAX_ARGS; i++) {
    if (cmd.arg_char[i] == letter) {
      return cmd.arg_value[i];
    }
  }
  return GCODE_ARG_VALUE_ERR;
} // getCurrentArgument

bool gcodeArgWasGivenFor(char letter, GcodeCommand_t cmd) {
  for (uint8_t i = 0; i < GCODE_MAX_ARGS; i++) {
    if (cmd.arg_char[i] == letter) {
      return true;
    }
  }
  return false;
} // gcodeArgWasGivenFor

bool gcodeNoArgsGiven(GcodeCommand_t cmd) {
  return (cmd.arg_char[0] == 0);
} // noArgsGiven

// TEST FUNCTIONS /////////////////////////////////////////////////////////////

#ifdef UNIT_LEVEL_TESTING

TestResult_t TEST_gcode() {
  TestResult_t accumulator;
  GcodeCommand_t cmd;
  TEST_ASSERT_EQUAL(gcodeNoArgsGiven(cmd), true, accumulator);
  cmd.arg_char[0] = 'a';
  cmd.arg_char[1] = 'b';
  cmd.arg_char[2] = 'z';
  TEST_ASSERT_EQUAL(gcodeArgWasGivenFor('a', cmd), true, accumulator);
  TEST_ASSERT_EQUAL(gcodeArgWasGivenFor('b', cmd), true, accumulator);
  TEST_ASSERT_EQUAL(gcodeArgWasGivenFor('z', cmd), true, accumulator);
  TEST_ASSERT_EQUAL(gcodeArgWasGivenFor('c', cmd), false, accumulator);
  cmd.arg_value[0] = 0;
  cmd.arg_value[1] = 255;
  // Don't know what it should be, but this (below) should represent the max expected value for a GCODE command.
  cmd.arg_value[2] = 1024;
  TEST_ASSERT_EQUAL(gcodeParseValueFor('c', cmd), GCODE_ARG_VALUE_ERR, accumulator);
  TEST_ASSERT_EQUAL(gcodeParseValueFor('a', cmd), 0, accumulator);
  TEST_ASSERT_EQUAL(gcodeParseValueFor('b', cmd), 255, accumulator);
  TEST_ASSERT_EQUAL(gcodeParseValueFor('z', cmd), 1024, accumulator);
  return accumulator;
} // TEST_gcode
#endif // ifdef UNIT_LEVEL_TESTING
