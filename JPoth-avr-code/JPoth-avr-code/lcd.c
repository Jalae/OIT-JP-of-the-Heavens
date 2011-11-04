/*
 * lcd.c
 *
 * Created: 10/26/2011 1:11:48 PM
 *  Author: nolan.check
 */ 

#include "lcd.h"

#include <avr/io.h>
#include <util/delay.h>

#define PA_LCD_RS_MASK (1 << 0)
#define PA_LCD_RW_MASK (1 << 1)
#define PA_LCD_E_MASK (1 << 2)
#define PA_LCD_DB_SHIFT 4
#define PA_LCD_DB_MASK (0xF << PA_LCD_DB_SHIFT)
#define PA_LCD_ALL_MASK (PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_E_MASK | PA_LCD_DB_MASK)

/* Our LCD is connected as follows:
 *  LCD Pin | MPU Pin
 *       RS | PA0
 *       RW | PA1
 *        E | PA2
 * DB<7..4> | PA<7..4>
 * The LCD operates in 4-bit mode.
 */

void lcd_set_db_input()
{
	// Disable pull-up for inputs
	PORTA &= ~PA_LCD_DB_MASK;
	DDRA &= ~PA_LCD_DB_MASK;
}

void lcd_signal_out(int rs, int rw, int db)
{
	// Set DB pins as outputs
	DDRA |= PA_LCD_DB_MASK;
	
	// TODO: Look for AVR features for manipulating PORTA atomically
	
	// Output signals on PORTA
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= ((rs?PA_LCD_RS_MASK:0) | (rw?PA_LCD_RW_MASK:0) | (db<<PA_LCD_DB_SHIFT));
}

int lcd_read_bf_and_address()
{
	return 0;
}

void lcd_wait_bf()
{
	// FIXME: This is NOT how to read busy-flag.
	PORTA |= PA_LCD_RW_MASK;
	while (PORTA & PA_LCD_RS_MASK)
		; // Do nothing
}

void lcd_write(int rs, int data)
{
	lcd_wait_bf();
	
	// High nibble
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= ((rs ? PA_LCD_RS_MASK : 0) | ((data >> 4) << PA_LCD_DB_SHIFT));
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= (PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~(PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	
	// Low nibble
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= ((rs ? PA_LCD_RS_MASK : 0) | ((data & 0xF) << PA_LCD_DB_SHIFT));
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= (PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~(PA_LCD_RS_MASK | PA_LCD_DB_MASK);
}	

void lcd_set_dd_ram_address(int addr)
{
	lcd_write(0, (1<<8) | addr);
}

void lcd_write_data_to_ram(int data)
{
	lcd_write(1, data);
}

void lcd_init()
{
	// Disable pull-up for inputs, output low for outputs
	PORTA &= ~PA_LCD_ALL_MASK;
	// RS, RW and E shall be outputs
	DDRA |= PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_E_MASK;
	// DB<7..4> shall be output only when necessary. They shall be input at all
	// other times.
	lcd_set_db_input();
	
	// See page 47 and 48 for Read and Write timing diagrams.
	// In 4-bit mode, each read and write is divided into two parts: one for
	// the high nibble and one for the low nibble.
	
	// Follow 4-Bit Initialization procedure on page 33 of datasheet.
	
	// Wait more than 15 ms after Vcc = 4.5V
	_delay_ms(15);
	
	// Function Set Command: (8-bit interface)
	// Send RS=0, RW=0, DB=0011
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= (0x3 << PA_LCD_DB_SHIFT);
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= PA_LCD_DB_MASK;
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~PA_LCD_DB_MASK;
	
	// Wait more than 4.1 ms
	_delay_ms(5);
	
	// Function Set Command: (8-bit interface) again!
	// Send RS=0, RW=0, DB=0011
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= (0x3 << PA_LCD_DB_SHIFT);
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= (PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~(PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	
	// Wait more than 100 us
	_delay_ms(1);
	
	// Function Set Command: (8-bit interface) yet again!
	// After this command is written, BF can be checked
	// Send RS=0, RW=0, DB=0011
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= (0x3 << PA_LCD_DB_SHIFT);
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= (PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~(PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	
	// Function Set: Sets interface to 4-bit
	// Send RS=0, RW=0, DB=0010
	lcd_wait_bf();
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= (0x2 << PA_LCD_DB_SHIFT);
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= (PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~(PA_LCD_RS_MASK | PA_LCD_DB_MASK);
	
	// Function Set: Interface=4-bit, Set N and F for number of characters and font
	lcd_write(0, 0x28);
	
	// Display OFF
	lcd_write(0, 0x08);
	
	// Clear Display
	lcd_write(0, 0x01);
	
	// Entry Mode Set:
	lcd_write(0, 0x04);
	
	// Display ON (Set C and B for cursor/blink options)
	lcd_write(0, 0x0F);
	
	// TESTING //
	
	// Display stuff
	lcd_set_dd_ram_address(0);
	lcd_write_data_to_ram('H');
	
	/////////////
}
