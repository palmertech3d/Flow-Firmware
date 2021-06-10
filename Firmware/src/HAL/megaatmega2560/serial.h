#ifndef SERIALH
#define SERIALH

#include <avr/io.h>

// Initializes USART0
void usart0_init(int);

// Send a char to USART0
void usart0_write_int(uint8_t);

void usart0_write_char(char data);

// Send a string to USART0
void usart0_write(uint8_t*);

void usart0_write_str(char*);
// Receive a char from USART0
uint8_t usart0_receive();

#endif // SERIALH
