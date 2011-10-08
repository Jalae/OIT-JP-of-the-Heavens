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
	PORTB ^= 2;

}

int main(void)
{
	sei();
	DDRB = 1<<PB1;
	PORTB = 0xFF;

	PCICR = 1<<PCIE1;
	PCMSK1 = 1<<PCINT8;
	//EIMSK = 1<<PCIE1;
	
	
	
	
	
	
    while(1)
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

