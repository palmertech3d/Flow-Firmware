#include <stdio.h>

// A struct to hold a gcode command
struct gcodeCommand{
  char letter; // G or an M
  int command; // Number of the gcode command
  char argChar[3]; // Letters for up to three arguments
  int argInt[3]; // Values for up to three arguments
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

    // Takes in a char* for an inputted string.
    // Returns a gcodeCommand object corresponding
    // to the gcode command parsed.
    // Returns a gcodeCommand object with property
    // char letter = -1 if inputed string is invalid.
    gcodeCommand parsegcode2(char*);

    // Cuts a string at the passed index, returning the string from the index
    // to a space or a null byte
    void cutString(char* input, char* output, int* index);

  private:

    int getIntFromString(char* input, int numPlaces);

    // Takes in a char, returns true if it is a num, false if not.
    bool isNum(char data);

};
