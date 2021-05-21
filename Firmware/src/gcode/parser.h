class parser{
  public:
    // Takes in a char* to an inputted string. Returns an int value
    // corresponding to a gcode command.
    // int values:
    // 1 + xxx = Gxxx
    // 2 + xxx = Mxxx
    int parsegcode(char*);

  private:
    
};
