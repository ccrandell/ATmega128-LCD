/* lcd.h
 */

#ifndef LCD_H_
#define LCD_H_
#include <stdint.h>

void LCD_cmd(uint8_t);
void delay_ms(unsigned int);
void LCD_nibble(uint8_t);
void LCD_data(uint8_t);
void LCD_POS(uint8_t, uint8_t);
void LCD_WR_Stg(const char*);
void LCD_WR_Int(int, int);
void LCD_CLEAR(void);
void lcd_initialize(void);

#endif 