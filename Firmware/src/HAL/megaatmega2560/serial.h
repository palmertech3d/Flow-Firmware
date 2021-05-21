#ifndef SERIALH
#define SERIALH

#include <avr/io.h>

// Initializes USART0
void usart0_init();

// Send a char to USART0
void usart0_transmit(unsigned char data);

// Received a char from USART0
unsigned char usart0_receive();
#endif // SERIALH
