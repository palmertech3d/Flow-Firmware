#ifndef GCODE_H
#define GCODE_H

#include "parser.h"
#include "queue.h"
#include "config_defaults.h"

typedef enum GcodeError_enum {
  ERR_SUCCESS,
  ERR_GCODE_INVALID,
  ERR_TOO_LONG,
  ERR_NO_SUCH_LETTER,
  ERR_NO_SUCH_NUMBER,
  ERR_NO_DATA
} GcodeError_t;

class gcode {
public:

// constructor
gcode();

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
queue buffer;

private:
// GcodeCommand_t active_command;
}

;

GcodeArg_t gcodeParseValueFor(char letter, GcodeCommand_t cmd);

#endif // GCODE_H
