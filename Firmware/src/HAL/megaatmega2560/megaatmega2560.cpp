#include "megaatmega2560.h"
#include "user_pins.h"
namespace HAL{

#define _U_SET_OUTPUT(IO) _SET_OUTPUT(IO)
#define _U_TOGGLE(IO) _TOGGLE(IO)
  void blinkLED(){
    //#define ENABLE 13
    SET_OUTPUT(ENABLE); // Set pin DIO13 to output mode

    TOGGLE(ENABLE); // Toggle pin DIO13
    _delay_ms(500);
    TOGGLE(ENABLE); // Toggle pin DIO13
    _delay_ms(500);

  }

};
