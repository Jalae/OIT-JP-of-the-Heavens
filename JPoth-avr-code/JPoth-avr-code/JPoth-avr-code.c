/*
 * JPoth_avr_code.c
 *
 * Created: 10/7/2011 2:28:09 PM
 *  Author: kehnin.dyer
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


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
		gotosleep(2);
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

