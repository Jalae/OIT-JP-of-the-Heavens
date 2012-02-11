/* Created By Tyler Martin 12/13/2011
Contains functions for using the UART to talk to the cellular module.
*/
//initializes the UART for the cellular module
//4800 bps, 8 data bits, no parity, 1 stop bit.
//#include "cellular.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif /* F_CPU */
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16))) - 1)




char rbuff[100];
//char tbuff[100];
char * rptr = rbuff;
char * tptr = 0;//= tbuff;
char TXdone = 1;
char RXdone = 1;

void uart_init()
{
	//UBRR0 = 259; //set baud to 4800, based on a clock of 20MHz, normal mode
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = (BAUD_PRESCALE);

	//set up frame, 8 data bits, no parity , 1 stop bit
	UCSR0C |= (0<<USBS0) | (1<<UCSZ10) | (1<<UCSZ00);	
	//enable receiver and transmitter
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	
	//enable receive and transmit interrupts
	UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);
//	sei();
}

/*
//sends a string as an AT command via UART
void ATsend(char * command)
{
	while(TXdone == 0);
	TXdone = 0;
	tptr = command;		
	UDR0 = * tptr;		
};
*/

void ATsend_no(char * s)
{
	tptr = s;
	while(*tptr != 0)
	{
		while((UCSR0A&(1<<UDRE0)) == 0);
		UDR0 = *tptr;
	//	lcd_puts("1");
		tptr++;
	}
	//lcd_puts("\n");		
}

/*
//Receives the result of the AT command
void ATrecive(char * message)
{
	while(RXdone == 0);
	strcpy(message,rbuff);
};
*/

void ATrecive_no(char * s)
{	
	int i = 1;
	rptr = s;
	
	do 
	{
		while((UCSR0A&(1<<RXC0)) == 0 && (i!=0) )
		i++;
		if(UDR0>=32)
		{
			*rptr = UDR0;
			lcd_putc(*rptr);
			rptr++;
			i=1;
		}
	
	}while (i!=0);
	*rptr = 0;
}	


/*
ISR(USART0_TX_vect)
{
	tptr++;
	if(*tptr != 0)//if not end of the string send next byte
	{
		UDR0 = *tptr;
	}
	else
	{
		
		TXdone = 1;
	}
}

ISR(USART0_RX_vect)
{
	RXdone = 0;
	*rptr = UDR0;
	if(*rptr == '\r')
	{
		*rptr = 0;
		
		rptr = rbuff;
		RXdone = 1;
	}
	else
	{
	rptr++;
	}	
}
*/