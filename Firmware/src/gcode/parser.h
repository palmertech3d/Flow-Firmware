#include <stdio.h>

// A struct to hold a gcode command
struct gcodeCommand{
  char letter; // G or an M
  uint8_t command; // Number of the gcode command

};

class parser{
  public:
    // Default constructor
    parser();

    // Takes in a char* for an inputted string. Returns an int value
    // corresponding to a gcode command.
    // int values:
    // 1 + xxx = Gxxx
    // 2 + xxx = Mxxx
    int parsegcode(char*);

  private:

};
