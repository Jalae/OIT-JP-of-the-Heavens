/*
 * JPoth_avr_code.c
 *
 * Created: 10/7/2011 2:28:09 PM
 *  Author: kehnin.dyer
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


ISR(PCINT1_vect)
{
	PORTB ^= 2;			//not pin 2 to toggle the light

}

int main(void)
{
	sei();				//enable interrupts
	DDRB = 1<<PB1;		//set pin 1 on port B to input
	PORTB = 0xFF;		//Activate internal pull up resistors on port B

	PCICR = 1<<PCIE1;	
	PCMSK1 = 1<<PCINT8;
	//EIMSK = 1<<PCIE1;
	
	
	
	
	
	
    while(1)		//do nothing to test ISR
    {
		//if(PINB & 0x01)
		//{
			//PORTB |= 0x02;
		//}			
		//else
		//{
			//PORTB &= ~0x02;
		//}			
        //TODO:: Please write your application code 
    }
}

