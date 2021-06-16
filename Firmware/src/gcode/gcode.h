#ifndef GCODEH
#define GCODEH

#include "parser.h"
#include "queue.h"

class gcode{
  public:
    // constructor
    gcode();

    // gets gcode from usart and put it in the buffer
    // returns 0 if valid gcode entered
    // returns 1 if invalid gcode entered
    int get_gcode();


    // executes the gcode command specified by command
    // returns true if command executed successfully
    // returns false if command executed unsuccessfully
    bool execute_gcode(gcodeCommand command);


    // decides what command is next to be executed in the buffer,
    // then calls execute_gcode to execute it
    // returns true if buffer executed successfully
    // returns false if buffer executed unsuccessfully
    bool execute_buffer();


    // gcode access functions
    void G28();

    // gcode command buffer
    queue buffer;
    
  private:


};

#endif // GCODEH
