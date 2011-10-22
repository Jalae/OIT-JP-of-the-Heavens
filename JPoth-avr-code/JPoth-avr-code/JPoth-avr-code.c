/*
 * JPoth_avr_code.c
 *
 * Created: 10/7/2011 2:28:09 PM
 *  Author: kehnin.dyer
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/delay.h>

#define TIME 3


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
//Steps the motor forward or backwards for a number of steps
//if dir > 0 forward else backwards
void step(char dir, int steps)
{
	PORTC = (PORTC | 0xC4);//set outputs to 0
	PORTD = (PORTD | 0xBF);
	DDRC = 0x3B;//set to outputs
	DDRD = 0x80;//(>,<)
	char state = 0;
	PORTC = PORTC & (1<<PC5 | 1<<PC1);
	
	if (dir > 0)//if greater than 0 forward
	{
		while(steps>0)	
		{
			switch (state)
			{
				case '0':
					state = 1;
					PORTC = PORTC & (1<<PC3 | 0<<PC4 | 0<<PC0);
					PORTD = PORTD & (0<<PD7);
					_delay_ms(TIME);
					break;
				case '1':
					state = 2;
					PORTC = PORTC & (0<<PC3 | 0<<PC4 | 1<<PC0);
					PORTD = PORTD & (0<<PD7);
					_delay_ms(TIME);
					break;
				case '2':
					state = 3;
					PORTC = PORTC & (0<<PC3 | 1<<PC4 | 0<<PC0);
					PORTD = PORTD & (0<<PD7);
					_delay_ms(TIME);
					break;
				case '3':
					state = 4;
					PORTC = PORTC & (0<<PC3 | 0<<PC4 | 0<<PC0);
					PORTD = PORTD & (1<<PD7);
					_delay_ms(TIME);
					break;
				default:
					state = 0;
					PORTC = PORTC & (0<<PC3 | 0<<PC4 | 0<<PC0);
					PORTD = PORTD & (0<<PD7);
					_delay_ms(TIME);	
					break;
			}
		}
	}
	else //go backwards
	{
		while(steps>0)
		{
			switch (state)
			{
				case '0':
					state = 3;
					PORTC = PORTC & (1<<PC3 | 0<<PC4 | 0<<PC0);
					PORTD = PORTD & (0<<PD7);
					_delay_ms(TIME);
					break;
				case '1':
					state = 0;
					PORTC = PORTC & (0<<PC3 | 0<<PC4 | 1<<PC0);
					PORTD = PORTD & (0<<PD7);
					_delay_ms(TIME);
					break;
				case '2':
					state = 1;
					PORTC = PORTC & (0<<PC3 | 1<<PC4 | 0<<PC0);
					PORTD =PORTD & (0<<PD7);
					_delay_ms(TIME);
					break;
				case '3':
					state = 2;
					PORTC = PORTC & (0<<PC3 | 0<<PC4 | 0<<PC0);
					PORTD = PORTD & (1<<PD7);
					_delay_ms(TIME);
					break;
				default:
					state = 0;
					PORTC = PORTC & (0<<PC3 | 0<<PC4 | 0<<PC0);
					PORTD = PORTD & (0<<PD7);	
					_delay_ms(TIME);
					break;
			}
		}
		
	}
	PORTC = PORTC & (0<<PC0 | 0<<PC1 | 0<<PC3 | 0<<PC4 | 0<<PC5);
	PORTD = PORTD & (0<<PD7);
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
		step(0,200);
		//gotosleep(2);
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

