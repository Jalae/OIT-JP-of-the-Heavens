/*
 * lcd.c
 *
 * Created: 10/26/2011 1:11:48 PM
 *  Author: nolan.check
 */ 

#include "lcd.h"

#include <avr/io.h>

#define PA_LCD_RS_MASK (1 << 0)
#define PA_LCD_RW_MASK (1 << 1)
#define PA_LCD_E_MASK (1 << 2)
#define PA_LCD_DB_MASK (0xF << 4)
#define PA_LCD_ALL_MASK (PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_E_MASK | PA_LCD_DB_MASK)

/* Our LCD is connected as follows:
 *  LCD Pin | MPU Pin
 *       RS | PA0
 *       RW | PA1
 *        E | PA2
 * DB<7..4> | PA<7..4>
 * The LCD operates in 4-bit mode.
 */

void lcd_init()
{
	// Disable pull-up for inputs, output low for outputs
	PORTA &= ~PA_LCD_ALL_MASK;
	// RW and E shall be outputs
	DDRA |= PA_LCD_RW_MASK | PA_LCD_E_MASK;
	// RS and DB<7..4> shall be output only when necessary. They shall be input
	// at all other times.
	DDRA &= ~(PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	
	// See page 47 and 48 for Read and Write timing diagrams.
	// In 4-bit mode, each read and write is divided into two parts: one for
	// the high nibble and one for the low nibble.
	
	// Follow 4-Bit Initialization procedure on page 33 of datasheet.
	
	// Wait more than 15 ms after Vcc = 4.5V
	// TODO
	
	// Function Set Command: (8-bit interface)
	// TODO
	
	// Wait more than 4.1 ms
	// TODO
	
	// Function Set Command: (8-bit interface)
	// TODO
	
	// Wait more than 100 us
	// TODO
	
	// Function Set Command: (8-bit interface)
	// After this command is written, BF can be checked
	// TODO
	
	// Function Set: Sets interface to 4-bit
	// TODO
	
	// Function Set: Interface=4-bit, Set N and F for number of characters and font
	// TODO
	
	// Display OFF
	// TODO
	
	// Clear Display
	// TODO
	
	// Entry Mode Set:
	// TODO
	
	// Display ON (Set C and B for cursor/blink options)
	// TODO
}
