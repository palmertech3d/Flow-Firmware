#ifndef GCODE_H
#define GCODE_H

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
    void g1(int motor, int speed);

    void g2(int motor);

    void g28();

    void m104(); // Turns off the hotend

    void m104(int temp); // Sets the temp of the hotend

    void m106();

    void m303();

    void m503();

    // gcode command buffer
    queue buffer;

  private:


};

#endif // GCODE_H
