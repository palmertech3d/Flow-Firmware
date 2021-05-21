#include "serial.h"

// define some macros
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

// function to initialize UART
void usart0_init(){
    UBRR0H = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRR0L = BAUDRATE;                           // set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0);                // enable receiver and transmitter
    UCSR0C|= (1<<UMSEL00)|(1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format
}

void usart0_transmit(unsigned char data){
  while (!(UCSR0A & (1<<UDRE0)));                // wait while register is free
  UDR0 = data;
}

unsigned char usart0_receive()
{
    while(!(UCSR0A) & (1<<RXC0));                   // wait while data is being received
    return UDR0;                                   // return 8-bit data
}
