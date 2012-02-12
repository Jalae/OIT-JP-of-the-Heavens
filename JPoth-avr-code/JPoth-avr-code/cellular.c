/*
 *cellular.c
 *
 *Created 02/12/2012
 *	Author:	Kehnin Dyer
 **************************************************************
 *Description: Contains all the functions for setting up and
 *controlling the Cell module
*/


void cellular_init()
{
	//configure pins
	DDRB = DDRB | (1 << PORTB1);//make Cell phone ON/OFF output
	DDRB = DDRB & (0 << PORTB2); //set pin 3 "poweron MON" to be input
	
	_delay_ms(500); //wait for these pins to be stable

	if(cellular_isOn())
	{
		ToggleCellModule(); //turn it off then!
		while(cellular_isOn()) //wait for it to turn off
		{
			//maybe add an escape condition
		}
	}
	if(!cellular_isOn()) //it's off i guess
	{
		ToggleCellModule();
		while(!cellular_isOn()) //wait for it to turn on
		{
			//maybe?
		}
	}
	ATsend_no("AT\r"); //get the modules attention.
	_delay_ms(7000);
	ATsend_no("AT\r"); //no really we want your attention.
	_delay_ms(5000);	//we need these delays, because some time during
						//the init process the module will send a MWI
						//and the next command we send will likely fail
						//if it is an AT then everything is fine.
						//however this can only increase our odds...
						//without real parsing we have to do this.
}

void cellular_sms_init()
{
	ATsend_no("AT+cmgf=1\r"); //set sms mode to be text
	_delay_ms(5000); //TODO:remove this once we can figure out if commands succeed.
	ATsend_no("AT+cmgf=1\r"); //make sure that this command is set for real.
	_delay_ms(1000); //give her some thinking room.
	ATsend_no("AT+csmp=17,167,0,16\r"); //this should be persistant, however, setting it every boot can't hurt.
	_delay_ms(1000);
}

void cellular_send_sms(char* msg, char* num)
{
	ATsend_no("AT+cmgs=\"+"); //at command for sms
	ATsend_no(num); //send the number
	ATsend_no("\"\r"); //finish the command
	_delay_ms(1000); //thinking time
	ATsend_no(msg);
	ATsend_no("\r\x1A\r");//send the end message command ascii 'SUB' or ^Z
	_delay_ms(5000); //more thinking time, this theoreticly can take up to 30 seconds.
}




int cellular_isOn()
{
	return (PINB & (1<<PINB2));
}


void ToggleCellModule()
{
	PORTB = PORTB | (1<<PORTB1);
	_delay_ms(1000);
	PORTB = PORTB & ~(1<<PORTB1);
}



