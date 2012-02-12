/*
 *cellular.h
 *
 *Created 02/12/2012
 *	Author:	Kehnin Dyer
 **************************************************************
 *Description: Contains all the functions for setting up and
 *controlling the Cell module
*/


/*
	configures pins for cellmodule control
	turns on the module
	readys the module for following AT commands
	has a mandatory delay of ~2 seconds
*/
void cellular_init();


/*
Configures the module to send texts
*/
void cellular_sms_init();

/*
Sends a text to the number given of the message given
number format:
	char number[] = "15551234567"
*/
void cellular_send_sms(char* msg, char* num);

//returns 1 if it is on, 0 if off
int cellular_isOn();



/*
	Toggles the state of ON/OFF of the cell module.

	no checking is done here. poweronmon should be checked
	for correct state change.
*/
void ToggleCellModule();





