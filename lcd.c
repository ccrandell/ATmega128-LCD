/*
 * lcd.c
 *
 * Created: 8/28/2014 11:42:42 AM
 * Authors: Claire Crandell, Fatima Bamarouf
 *
 * Reference:
 * homepage.hispeed.ch/peterfleury/lcdlibrary.zip
 */ 

//PA0-PA3: LCD DB4-DB7
//PA4: Enable (E)
//PA5: Read/Write (R/W, write active low)
//PA6: Register Select (RS, 0=instruction, 1=data)
//PA7: unused

#include <avr/io.h>
#include <string.h>

#define XTAL 16000000L

static inline void _delayFourCycles(unsigned int __count)
{
	__asm__ __volatile__ (
	"1: sbiw %0,1" "\n\t"
	"brne 1b"                              // 4 clock cycles/loop
	: "=w" (__count)
	: "0" (__count)
	);
}

#define delay_us(microseconds) _delayFourCycles( ( ( 1*(XTAL/4000) )*microseconds)/1000 )

void delay_ms(unsigned int milliseconds)
{
	while(milliseconds>0)
	{
		for(volatile unsigned int counter=940; counter>0; counter--); //each loop takes 17 clock cycles
		milliseconds--;
	}
}

void LCD_cmd(uint8_t instruction)
{
	PORTA=0x00;
	PORTA&=0xBF; //clear PA6
	PORTA|=0x10; //set Enable
	PORTA|=(instruction>>4); //First nibble
	delay_us(1); //Data setup
	PORTA&=0xEF; //clear Enable
	delay_us(1); //Data hold time
	PORTA&=0xF0; //clear nibble
	
	PORTA=0x10; //set Enable
	PORTA|=(0x0F&instruction); //Second nibble
	delay_us(1); //Data setup
	PORTA&=0xEF; //clear Enable
	delay_us(1); //Data hold time
	PORTA&=0xF0; //clear nibble
}

void toggle_enable(void)
{
	PORTA|=0x10; //set Enable
	delay_us(5); //Enable rise
	PORTA&=0xEF; //clear Enable	
}

void LCD_nibble(uint8_t instruction)
{
	PORTA=0x00;
	PORTA&=0xBF; //clear PA6
	PORTA|=0x10; //set Enable
	PORTA|=(0x0F&instruction); //Second nibble
	delay_us(1); //Data setup
	PORTA&=0xEF; //clear Enable
	delay_us(1); //Data hold time
	PORTA=0x00; //clear nibble
}

void LCD_data(uint8_t data)
{
	PORTA=0x00;
	PORTA|=0x40; //set PA6
	PORTA|=0x10; //set Enable
	PORTA|=(data>>4); //First nibble
	delay_us(1); //Data setup
	PORTA&=0xEF; //clear Enable
	delay_us(1); //Data hold time
	PORTA&=0xF0; //clear nibble
	
	PORTA|=0x10; //set Enable
	PORTA|=(0x0F&data); //Second nibble
	delay_us(1); //Data setup
	PORTA&=0xEF; //clear Enable
	delay_us(1); //Data hold time
	PORTA&=0xF0; //clear nibble
}

void LCD_POS(uint8_t char_pos, uint8_t line)
{
	if(char_pos<=0x1F && line==0)
	{
		char_pos|=0x80; //set PA7
		LCD_cmd(char_pos);
		delay_us(60);
	}
	if(char_pos>=0x40 && char_pos<=0x5F && line==1)
	{
		char_pos|=0x80; //set PA7
		LCD_cmd(char_pos);
		delay_us(60);
	}	
}

void LCD_WR_Stg(const char* input)
{
	for(int i=0; i<strlen(input); i++)
	{
		LCD_data(input[i]);	
		delay_us(60);	
	}	
}

void LCD_WR_Int(int variable, int length)
{
	uint8_t digits[length];
	
	for(int i=0;i<length;i++)
	{
		digits[i]=variable%10;
		variable/=10;
	}
	
	for(int i=length-1; i>=0; i--)
	{
		LCD_data(digits[i]+0x30); //offset needed to convert to ASCII
		delay_us(60);
	}
}

void LCD_CLEAR(void)
{
	LCD_cmd(0x01); //clear display
	delay_ms(3);
	LCD_cmd(0x02); //return home
	delay_ms(3);
}

void lcd_initialize(void)
{
	DDRA=0xFF;	//set port A for output
	PORTA=0x00; //disable PA5 for write mode
	
	delay_ms(250);
	LCD_nibble(0x03);
		
	delay_ms(7); //4.1 ms * 1.5 = 6.15 ms minimum
	LCD_nibble(0x03);
		
	delay_us(150); //100 us * 1.5 = 150 us minimum
	LCD_nibble(0x03);
		
	delay_us(100);
	LCD_nibble(0x02);
	
	delay_us(150);
	LCD_cmd(0x28); //N (DB7) = 1, 2 lines; F (DB6) = 0, 5x8
	
	delay_us(60); //max delay: 40 us * 1.5 = 60 us
	LCD_cmd(0x08);
	 
	delay_us(60);
	LCD_cmd(0x01);
	
	delay_ms(3);
	LCD_cmd(0x06); //I/D (DB5): 1, increment, S (DB4): 0, no shift
	
	delay_us(60);
	LCD_cmd(0x0F); //D (DB2): 1, display on, C (DB1): 1, cursor on, B (DB0) 1, blink on
	
	delay_us(60);
	LCD_CLEAR();
	delay_ms(3);
	
	delay_us(60);
	LCD_POS(0x00, 0); //first line
	delay_us(60);
	LCD_WR_Stg("Bradley Univ.");
	
	delay_us(60);
	LCD_POS(0x40, 1); //second line
	delay_us(60);
	LCD_WR_Stg("ECE Department");
	
	delay_us(60);
	LCD_POS(0x10, 0); //third line
	delay_us(60);
	LCD_WR_Stg("ECE 322 Tach Proj");
	
	delay_us(60);
	LCD_POS(0x50, 1); //fourth line
	delay_us(60);
	LCD_WR_Stg("Sep. 9, 2014");
	
	delay_ms(2000);
	LCD_CLEAR();
	delay_ms(3);
}