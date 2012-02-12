/*
 *ancient.c
 *
 *Created 02/12/2012
 *	Author:	Kehnin Dyer
 **************************************************************
 *Description: Contains all the functions that likely won't be used
*/


void gotosleep(char mode)
{
	SMCR = (mode << 1) | 1; //set Sleep mode control register to mode 1 or "power down",
		  //sleep mode enabled
	asm("SLEEP"); //is this an assembly instruction?
}



