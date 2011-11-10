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

static int lcd_read(int rs)
{
	int result;
	
	// DB pins are assumed to be configured as high-Z inputs
	
	// Set RS as indicated
	// Set RW high
	if (rs) {
		PORTA |= (PA_LCD_RS_MASK | PA_LCD_RW_MASK);
	} else {
		PORTA &= ~PA_LCD_RS_MASK;
		PORTA |= PA_LCD_RW_MASK;
	}
	// Delay tAS
	_delay_ms(1);
	
	// Set E high
	PORTA |= PA_LCD_E_MASK;
	// Delay tDDR
	_delay_ms(1);
	// Read upper nibble
	result = ((PINA & PA_LCD_DB_MASK) >> PA_LCD_DB_SHIFT) << 4;
	// Set E low
	PORTA &= ~PA_LCD_E_MASK;
	// Delay tH
	_delay_ms(1);
	
	// Set E high
	PORTA |= PA_LCD_E_MASK;
	// Delay tDDR
	_delay_ms(1);
	// Read lower nibble
	result |= ((PINA & PA_LCD_DB_MASK) >> PA_LCD_DB_SHIFT);
	// Set E low
	PORTA &= ~PA_LCD_E_MASK;
	// Delay tH
	_delay_ms(1);
	
	return result;
}

static int lcd_read_bf_and_address()
{
	return lcd_read(0);
}

static void lcd_wait_bf()
{
	while (lcd_read_bf_and_address() & 0x80)
		; // Wait for BF to be clear
}

static void lcd_write(int rs, int db)
{
	lcd_wait_bf();
	
	// Set RS as indicated
	// Set RW low
	if (rs) {
		PORTA |= PA_LCD_RS_MASK;
		PORTA &= ~PA_LCD_RW_MASK;
	} else {
		PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK);
	}
	// Delay tAS
	_delay_ms(1);
	
	// Set E high
	// Set DB as high nibble
	// Set DB pins as outputs
	PORTA |= PA_LCD_E_MASK;
	PORTA &= ~PA_LCD_DB_MASK;
	PORTA |= ((db >> 4) << PA_LCD_DB_SHIFT);
	DDRA |= PA_LCD_DB_MASK;
	// Delay tDSW
	_delay_ms(1);
	// Set E low
	PORTA &= ~PA_LCD_E_MASK;
	// Delay tH
	_delay_ms(1);
	
	// Set E high
	// Set DB as low nibble
	PORTA |= PA_LCD_E_MASK;
	PORTA &= ~PA_LCD_DB_MASK;
	PORTA |= ((db & 0xF) << PA_LCD_DB_SHIFT);
	// Delay tDSW
	_delay_ms(1);
	// Set E low
	PORTA &= ~PA_LCD_E_MASK;
	// Delay tH
	_delay_ms(1);
	
	// Set DB pins as high-Z inputs
	PORTA &= ~PA_LCD_DB_MASK;
	DDRA &= ~PA_LCD_DB_MASK;
}

static void lcd_set_dd_ram_address(int addr)
{
	lcd_write(0, (1<<7) | addr);
}

void lcd_init()
{
	// Disable pull-up for inputs, output low for outputs
	PORTA &= ~PA_LCD_ALL_MASK;
	// DB<7..4> shall be output only when necessary. They shall be input at all
	// other times.
	DDRA &= ~PA_LCD_DB_MASK;
	// RS, RW and E shall be outputs
	DDRA |= PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_E_MASK;
	
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
	DDRA &= ~PA_LCD_DB_MASK;
	
	// Wait more than 100 us
	_delay_ms(1);
	
	// Function Set Command: (8-bit interface) yet again!
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
	
	// Function Set: Sets interface to 4-bit
	// Send RS=0, RW=0, DB=0010
	PORTA &= ~(PA_LCD_RS_MASK | PA_LCD_RW_MASK | PA_LCD_DB_MASK);
	PORTA |= (0x2 << PA_LCD_DB_SHIFT);
	_delay_ms(1);
	PORTA |= PA_LCD_E_MASK;
	DDRA |= PA_LCD_DB_MASK;
	_delay_ms(1);
	PORTA &= ~PA_LCD_E_MASK;
	_delay_ms(1);
	DDRA &= ~PA_LCD_DB_MASK;
	
	// BF can now be checked.
	
	// Function Set: Interface=4-bit, Set N and F for number of characters and font
	lcd_write(0, 0x28);
	
	// Display OFF
	lcd_display_on_off(0, 0, 0);
	
	// Clear Display
	lcd_clear_display();
	
	// Set Entry Mode (initial configuration here)
	lcd_set_entry_mode(1, 0);
	
	// Display ON (initial configuration here)
	lcd_display_on_off(1, 1, 1);
	
	// TESTING //
	
	// Display stuff
	lcd_set_dd_ram_address(0);
	lcd_putc('H');
	lcd_putc('e');
	lcd_putc('l');
	lcd_putc('l');
	lcd_putc('o');
	
	/////////////
}

void lcd_clear_display()
{
	lcd_write(0, 0x01);
}

void lcd_set_entry_mode(int id, int s)
{
	lcd_write(0, 0x4 | (id?0x2:0) | (s?0x1:0));
}

void lcd_display_on_off(int d, int c, int b)
{
	lcd_write(0, 0x8 | (d?0x4:0) | (c?0x2:0) | (b?0x1:0));
}

void lcd_putc(char c)
{
	 lcd_write(1, c);
}
