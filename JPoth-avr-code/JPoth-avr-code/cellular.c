/* Created By Tyler Martin 12/13/2011
Contains functions for using the UART to talk to the cellular module.
*/
//initializes the UART for the cellular module
//4800 bps, 8 data bits, no parity, 1 stop bit.
#include "cellular.h"
void uart_init()
{
	//UBRR0 = 259; //set baud to 4800, based on a clock of 20MHz, normal mode
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = (BAUD_PRESCALE);
	//enable receiver and transmitter
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	//set up frame, 8 data bits, 1 stop bit
	UCSR0C |= (0<<USBS0) | (3<<UCSZ00);	
	//enable receive and transmit interrupts
	UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);
	sei();
}
//sends a string as an AT command via UART
void ATsend(char * command)
{
	while(TXdone == 0);
	TXdone = 0;
	tptr = command;		
	UDR0 = * tptr;		
};

//Receives the result of the AT command
void ATrecive(char * message)
{
	while(RXdone == 0);
	strcpy(message,rptr);
};

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
	if(*rptr == 0)
	{
		rptr = rbuff;
		RXdone = 1;
	}
	else
	{
	rptr++;
	}	
}