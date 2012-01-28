#include <avr/interrupt.h>
#include <avr/io.h>
#ifndef __CELLULAR_H_
#define __CELLULAR_H_
#ifndef F_CPU
#define F_CPU 10000000UL
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

char rbuff[100];
char tbuff[100];
char * rptr;
char * tptr;
char TXdone = 1;
char RXdone = 1;
//initializes the UART for the cellular module
//4800 bps, 8 data bits, no parity, 1 stop bit.
void uart_init();
//sends a string as an AT command via UART
void ATsend(char * command);

//Receives the result of the AT command
void ATrecive(char * message);
#endif /* __CELLULAR_H_ */
#endif