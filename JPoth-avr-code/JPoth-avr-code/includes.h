/*
 *includes.h
 *
 *Created 02/12/2012
 *	Author:	Kehnin Dyer
 **************************************************************
 *Description: Contains all the includes for main
*/
#ifndef F_CPU
#define F_CPU 8000000UL
#endif /* F_CPU */

#ifndef _DEFINES___
#define _DEFINES___

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/sleep.h>
#include <util/delay.h>
#include "motor.h"
#include "lcd.h"
#include "uart.h"
#include "cellular.h"
//#include "ancient.h"


#endif


