/*
 * lcd.h
 *
 * Created: 10/26/2011 1:11:25 PM
 *  Author: nolan.check
 */ 


#ifndef LCD_H_
#define LCD_H_

void lcd_init();
void lcd_clear_display();

// lcd_set_entry_mode: Controls what happens when a character is entered.
//   id: 1 for increment, 0 for decrement
//   s: 1 to shift entire display and hold cursor still, 0 to move cursor
void lcd_set_entry_mode(int id, int s);

// lcd_display_on_off
//   d: 1 to turn on display, 0 to turn off display
//   c: 1 to display cursor, 0 to hide cursor
//   b: 1 to blink cursor, 0 to not blink cursor
void lcd_display_on_off(int d, int c, int b);

void lcd_putc(char c);

#endif /* LCD_H_ */
