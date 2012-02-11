/*
 * JPoth_avr_code.c
 *
 * Created: 10/7/2011 2:28:09 PM
 *  Author:   	Kehnin Dyer
 * 		Tyler Martin
*/ 
#ifndef F_CPU
#define F_CPU 8000000UL
#endif /* F_CPU */

#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/sleep.h>
#include <util/delay.h>
#include "motor.h"
#include "lcd.h"
#include "cellular.h"



ISR(PCINT1_vect)
{
	PORTB ^= 2;			//not pin 2 to toggle the light
}

/*
for our purposes we can actually put the uC all the way to "power down" mode
this is accomplished by writing 010b to SM<2,0>. In this mode the core is off
until a pin state change interrupt is triggered. as this is the only kind of
interrupt we are using, and we don't need to handle anything until an interrupt
is triggered, this is the best mode for us. 
After the core wakes up it will wait 4 cycles then run the ISR associated with
the waking interrupt then continue running where the sleep left off. thus in main
we should have while(1)gotosleep(); or something similar.
for sleep mode to be bullet-proof we may need to write a reset vector.
mode can be 0 - 7
0 - idle
1 - ADC Noise Reduction
2 - Power-Down
3 - Power-save
4 - RES
5 - RES
6 - Standby
7 - Extended Standby
*/
void gotosleep(char mode)
{
	SMCR = (mode << 1) | 1; //set Sleep mode control register to mode 1 or "power down",
		  //sleep mode enabled
	asm("SLEEP"); //is this an assembly instruction?
}

void activateEmitter()
{
	PORTD =  PORTD|(1<<PORTD4);
}

void deactivateEmitter()
{
	PORTD = PORTD&(~(1<<PORTD4));
}

void ToggleCellModule()
{
	PORTB = PORTB | (1<<PORTB1);
	_delay_ms(1000);
	PORTB = PORTB & ~(1<<PORTB1);
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
	lcd_puts("Setting up");
	_delay_ms(5000);
	_delay_ms(1000);

	
	DDRB = DDRB | (1 << PORTB1);//make Cell phone ON/OFF output
	DDRB = DDRB & (0 << PORTB2); //set pin 3 "poweron MON" to be input
	
	_delay_ms(1000); //wait for stability
	lcd_puts("|");
	
	if((PINB & (1<<PINB2))) // is it on?
	{
		ToggleCellModule(); //turn it off.
		int i = 1;
		while((PINB & (1<<PINB2)))
		{
			i++;
			if(i == 0)		//egads we done timed out
			{
				ToggleCellModule();			
			}
		} 
	}
	lcd_puts(".");
	//delay_var_ms(1000);
	
	if(!(PINB & (1<<PINB2))) // is it off?
	{
		ToggleCellModule();	//turn it on
		int i = 1;
		while(!(PINB & (1<<PINB2)))
		{
			i++;
			if(i == 0)
			{
				ToggleCellModule();			
			}
		} 
	}
char message[100];

lcd_clear_display();

_delay_ms(1000);

ATsend_no("AT\r");
lcd_puts("AT\n");
_delay_ms(1000);
//ATrecive_no(message);
//lcd_puts(message);
lcd_clear_display();
ATsend_no("AT+cmgf=1\r");
lcd_puts("cmf");
_delay_ms(1000);
ATsend_no("AT+csmp=17,167,0,16\r");
lcd_puts(".csm");
_delay_ms(1000); //ctrl-z == 26 decimal ==\cz
ATsend_no("AT+cmgs=\"+15035070650\"\r");
lcd_puts("\n.mgs");
_delay_ms(1000);
ATsend_no("Omg did this work?\r\cz\r");
_delay_ms(1000);
lcd_puts("\nall clear");

lcd_puts("we got here");
	
//we should have the cell module running at this point

while(1)
{
	
	
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

