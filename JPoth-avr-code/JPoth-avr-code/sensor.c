/*
 *sensor.c
 *
 *Created 02/12/2012
 *	Author:	Kehnin Dyer
 **************************************************************
 *Description: Contains the functions for operating the emitters
*/

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
//
//i really don't know what was going on here...
//
char readSensor()
{
	PIND5; //empty
	PIND6;//dispensed
}

