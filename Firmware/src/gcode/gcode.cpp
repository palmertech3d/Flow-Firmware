#include "gcode.h"

gcode::gcode(){};

void gcode::G28(){
  gcode::homeLevelWinder();
}
