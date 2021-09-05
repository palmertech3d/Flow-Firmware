#include "megaatmega2560.h"
namespace HAL{

  void blinkLED(){
    //#define ENABLE 13
    SET_OUTPUT(PIN_ONBOARD_LED); // Set pin DIO13 to output mode

    TOGGLE(PIN_ONBOARD_LED); // Toggle pin DIO13
    _delay_ms(500);
    TOGGLE(PIN_ONBOARD_LED); // Toggle pin DIO13
    _delay_ms(500);

  }

};
