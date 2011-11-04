#include <avr/io.h>
#include <util/delay.h>

//configure all pins associated with the motor to be digital output
void motor_init()
{
	char DJT = MCUCR|(1<<JTD);
	MCUCR = DJT;//turn off jtag
	MCUCR = DJT;//turn off JTAG NOAU

	PORTC = (PORTC & (~0x3F));//set outputs to 0
	DDRC = DDRC | 0x3F;//set to outputs

}


//hax to delay a variable amount.
void delay_var_ms(int time)
{
	for(int i = 0; i < time; i++)
	{
		_delay_ms(1);
	}
}

//Steps the motor forward or backwards for a number of steps
//if dir > 0 forward else backwards
//TIME per step is TIME
void motor_step(char dir, int steps, int TIME)
{
	char state = 10; //go to an invalid state to setup.
	PORTC = PORTC | 0x22;//set the enable pins high
	for(int i = 0;i<steps;i++)
	{
		switch (state)
		{
		case 0:
			state = (dir?1:7);
			PORTC = PORTC ^ (0<<PC4 | 0<<PC3 | 1<<PC2 | 0<<PC0);
			delay_var_ms(TIME);
			break;
		case 1:
			state = (dir?2:0);
			PORTC = PORTC ^ (1<<PC4 | 0<<PC3 | 0<<PC2 | 0<<PC0);
			delay_var_ms(TIME);
			break;
		case 2:
			state = (dir?3:1);
			PORTC = PORTC ^ (0<<PC4 | 1<<PC3 | 0<<PC2 | 0<<PC0);
			delay_var_ms(TIME);
			break;
		case 3:
			state = (dir?4:2);
			PORTC = PORTC ^ (0<<PC4 | 0<<PC3 | 1<<PC2 | 0<<PC0);
			delay_var_ms(TIME);
			break;
		case 4:
			state = (dir?5:3);
			PORTC = PORTC ^ (0<<PC4 | 0<<PC3 | 0<<PC2 | 1<<PC0);
			delay_var_ms(TIME);
			break;
		case 5:
			state = (dir?6:4);
			PORTC = PORTC ^ (0<<PC4 | 1<<PC3 | 0<<PC2 | 0<<PC0);
			delay_var_ms(TIME);
			break;
		case 6:
			state = (dir?7:5);
			PORTC = PORTC ^ (1<<PC4 | 0<<PC3 | 0<<PC2 | 0<<PC0);
			delay_var_ms(TIME);
			break;
		case 7:
			state = (dir?0:6);
			PORTC = PORTC ^ (0<<PC4 | 0<<PC3 | 0<<PC2 | 1<<PC0);
			delay_var_ms(TIME);
			break;

		default:
			state = (dir?0:7);//send us to the right state based on direction
			PORTC = PORTC & (~(1<<PC4 | 1<<PC3 | 1<<PC2 | 1<<PC0));
			PORTC = PORTC | (1<<PC4);// put us in a valid state
			//delay_var_ms(TIME);
			break;
		}
	}
	PORTC = PORTC & (~0x3F);
}
