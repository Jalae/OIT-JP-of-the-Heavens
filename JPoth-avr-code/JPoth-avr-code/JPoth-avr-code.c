/*
 * JPoth_avr_code.c
 *
 * Created: 10/7/2011 2:28:09 PM
 *  Author:	Kehnin Dyer
 * 			Tyler Martin
*/ 

#include "includes.h"





void activateEmitter()
{
	PORTD =  PORTD|(1<<PORTD4);
}

void deactivateEmitter()
{
	PORTD = PORTD&(~(1<<PORTD4));
}


//
//return value
// XXXXXX<Bit for Empty><Bit for >
char readSensor()
{
	PIND5; //empty
	PIND6;//dispensed
}

int main(void)
{
	//first thing is to assert the relay. KEEP US ALIVE. I'm still alive!
	DDRC =  DDRC | (1 << PORTC7); //make powerctrl output
	PORTC = PORTC | (1 << PORTC7);// assert powerctrl
	
	uart_init();
	lcd_init();
	cellular_init();

	lcd_clear_display();
	_delay_ms(1000);

	cellular_sms_init();



	lcd_clear_display();
	lcd_puts("sending message");
	cellular_send_sms("this is a test", "1801413597");
	lcd_puts("\ncomplete");


	while(1)
	{
		//ATsend_no("U");
	}
/*	
//	char thing[100];
//	uart_init();
//	ATsend("test");
//	ATrecive(thing);
	lcd_init();
	sei();	
	//DDRB = 1<<PORTB1;		//set pin 1 on port B to input
	//PORTB = 0xFF;		//Activate internal pull up resistors on port B


	PORTD = 0;
	DDRD = DDRD&(~(1<<PORTD0 | 1<<PORTD4));

	//EIMSK = 1<<PCIE1;	
	
	sei();				//enable interrupts
//	PCICR = 1<<PCIE1;	
//	PCMSK1 = 1<<PCINT8;
	motor_init();
    
	
	delay_var_ms(5000);
	//PORTC = PORTC & (0 << PORTC7); //kill myself
	
	
	while(1)		//do nothing to test ISR
	{
		//gotosleep(2);
		
		motor_step(0,200,2);
	}
	*/
}

