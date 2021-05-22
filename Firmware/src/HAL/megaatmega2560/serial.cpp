#include "serial.h"

// define some macros
#define USART_BAUDRATE 9600
//#define UBRR_VAL

// function to initialize UART
void usart0_init(int baudrate){
  UBRR0 = ((F_CPU/(baudrate*16ul)) - 1); // Set baudrate
  UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01); // Set frame format to 8 data bits, no parity, 1 stop bit
  UCSR0B|= (1<<TXEN0)|(1<<RXEN0); // Enable both transmission and reception
}

void usart0_write_int(uint8_t data){
  while (!(UCSR0A & (1<<UDRE0))); // wait while register is free
  UDR0 = data;
}

void usart0_write_char(char data){
  while (!(UCSR0A & (1<<UDRE0))); // wait while register is free
  UDR0 = data;
}

void usart0_write(uint8_t* data){
  int i = 0;
  while (data[i] != 0){
    usart0_write_int(data[i]);
    i++;
  }
}

void usart0_write_str(char* data){
  int i = 0;
  while (data[i] != 0){
    usart0_write_char(data[i]);
    i++;
  }
}

uint8_t usart0_receive()
{
    while(!((UCSR0A) & (1<<RXC0)));                   // wait while data is being received
    return UDR0;                                   // return 8-bit data
}
