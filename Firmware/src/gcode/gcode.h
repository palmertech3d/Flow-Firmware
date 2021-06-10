class gcode{
  public:
    // constructor
    gcode();

    // gets gcode from usart and put it in the buffer
    // returns 1 if gcode was put in the buffer
    // returns 0 if gcode was not put in the buffer
    int get_gcode();

    // executes the next gcode due to run in the buffer
    // returns 1 if gcode successfully executed
    // returns 0 if gcode failed to execute
    int execute_buffer();


    // gcode access functions
    void G28();

  private:
    // G28 Functions
    void home_level_winder();

    // gcode buffer/queue
    int* buffer;

};
