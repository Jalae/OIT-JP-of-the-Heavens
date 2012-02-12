
#ifndef __CELLULAR_H_
#define __CELLULAR_H_


//initializes the UART for the cellular module
//4800 bps, 8 data bits, no parity, 1 stop bit.
void uart_init();
//sends a string as an AT command via UART
void ATsend(char * command);

void ATsend_no(char * s);

//Receives the result of the AT command
void ATrecive(char * message);

void ATrecive_no(char * s);

void CleanMessage(char* s);

#endif /* __CELLULAR_H_ */