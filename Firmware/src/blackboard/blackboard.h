/**
 * @Author: Nick Steele
 * @Date:   14:16 Oct 01 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 14:44 Oct 01 2021
 */
#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <stdint.h>

#define BBFLAG_BELOW_EXTRUSION_MINTEMP 0ul

class Blackboard_t {
private:
uint8_t global_flags;

public:

/// Get the specified flag
bool getFlag(uint8_t flag) {
  switch (flag) {
  case BBFLAG_BELOW_EXTRUSION_MINTEMP:
    return (this->global_flags & (1ul << BBFLAG_BELOW_EXTRUSION_MINTEMP));
    break;
  default:
    return 0;
  } // switch (flag)
} // getFlag

/// Set the specified flag
void setFlag(uint8_t flag, bool value) {
  switch (flag) {
  case BBFLAG_BELOW_EXTRUSION_MINTEMP:
    if (value) {
      this->global_flags |= (1ul << BBFLAG_BELOW_EXTRUSION_MINTEMP);
    } else {
      this->global_flags &= ~(1ul << BBFLAG_BELOW_EXTRUSION_MINTEMP);
    }
    break;
  } // switch (flag)
} // getFlag

Blackboard_t () {
  this->global_flags = 0;
}
} // class Blackboard_t

;

extern Blackboard_t global_blackboard;

#endif /* end of include guard: BLACKBOARD_H */
