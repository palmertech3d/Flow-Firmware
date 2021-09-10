#ifndef GCODE_H
#define GCODE_H

#include "parser.h"
#include "queue.h"
#include "config_defaults.h"
#include "logger.h"

typedef enum GcodeError_enum {
  GCODE_ERR_SUCCESS,
  GCODE_ERR_GCODE_INVALID,
  GCODE_ERR_TOO_LONG,
  GCODE_ERR_NO_SUCH_LETTER,
  GCODE_ERR_NO_SUCH_NUMBER,
  GCODE_ERR_NO_DATA
} GcodeError_t;

class GcodeExecuter {
public:

// constructor
GcodeExecuter();

// gets gcode from usart and put it in the buffer
GcodeError_t getGcode();

// executes the gcode command specified by command
GcodeError_t executeGcode(GcodeCommand_t cmd);

// decides what command is next to be executed in the buffer,
// then calls executeGcode to execute it
GcodeError_t executeBuffer();

// gcode access functions
void g1(GcodeCommand_t cmd);

void g2(GcodeCommand_t cmd);

void g28(GcodeCommand_t cmd);

void m104(GcodeCommand_t cmd);

void m106(GcodeCommand_t cmd);

void m303(GcodeCommand_t cmd);

void m503(GcodeCommand_t cmd);

// gcode command buffer
GcodeQueue buffer;

private:
// GcodeCommand_t active_command;
}

;

/**
 * Returns the value of a given letter, or GCODE_ARG_VALUE_ERR if no value was
 * sent or an invalid number was entered or the arguemnt does not exist.
 */
GcodeArg_t gcodeParseValueFor(char letter, GcodeCommand_t cmd);

/**
 * Returns true when any argument was given for a certain letter, even blank values.
 */
bool gcodeArgWasGivenFor(char letter, GcodeCommand_t cmd);

/**
 * Returns true when no args were given.
 */
bool gcodeNoArgsGiven(GcodeCommand_t cmd);

#endif // GCODE_H
