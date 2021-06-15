#ifndef GCODEH
#define GCODEH

#include "parser.h"
class gcode{
  public:
    // constructor
    gcode();

    // gets gcode from usart and put it in the buffer
    // returns 0 if valid gcode entered
    // returns 1 if invalid gcode entered
    int get_gcode();


    // executes the gcode command specified by command
    // returns 0 if command executed successfully
    // returns 1 if command executed unsuccessfully
    int execute_gcode(gcodeCommand command);


    // decides what command is next to be executed in the buffer,
    // then calls execute_gcode to execute it
    // returns 0 if buffer executed successfully
    // returns 1 if buffer executed unsuccessfully
    int execute_buffer();


    // gcode access functions
    void G28();

  private:
    // gcode buffer/queue
    int* buffer;

};

#endif // GCODEH
