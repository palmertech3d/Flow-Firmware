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
    // to the end of the string.
    char* cutString(char* input, int index);
    
  private:

    // Takes in a char* for an inputted string.
    // The first char of the string MUST be a number.
    // All other chars can be any ASCII character.
    //
    // The function scans the string until it finds a space or reaches
    // the max number of places to return, and then returns the
    // number scanned as an int.
    //
    // If the function finds a non-number character (not 0-9) while scanning,
    // it will return -1.
    //
    // The int argument specifies the number of integer places to return.
    //
    // The index argument specifies where to start scanning in the
    // passed string. This argument is passed by reference.
    int getIntFromString(char* input, unsigned int &index, int numPlaces);



};
