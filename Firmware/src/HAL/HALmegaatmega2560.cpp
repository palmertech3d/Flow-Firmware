#include "HALmegaatmega2560.h"

namespace HAL{

  void blinkLED(){
    DDRB = 0x00; // Reset input/output of PORTB
    DDRB |= 0x80; // Set PB7 as output

    PORTB |= 0x80; // Turn LED on by writing 1 to PB7
    _delay_ms(500);
    PORTB &= 0x7F;
    _delay_ms(500);

  }

};
