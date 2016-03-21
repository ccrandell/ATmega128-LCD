/*
 * main.c
 *
 * Created: 8/28/2014 11:42:42 AM
 * Authors: Claire Crandell, Fatima Bamarouf 
 *
 */

#include "bios_timer_int.h"
#include "lcd.h"
#include "bios_led8.h"
#include <avr/interrupt.h>

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

static volatile uint8_t  led_flag=0;
static volatile uint8_t deb_count = 50;
static volatile uint8_t deb_flag=0;
static int rpm=0;
static int enc_count=0;
	
ISR(TIMER2_COMP_vect)
{
	static uint16_t count_150ms=0;
	
	TCCR3B = 0x00; //pause timer
	enc_count=TCNT3L; //grab count
	rpm = enc_count*6; //calculate rpm
	TCNT3L=0; //reset count
	
	TCCR3B=TCCR3B|(1<<CS32)|(1<<CS31)|(1<<CS30); //restart counter
	PORTE=PORTE^(0x08); //toggle PE3
	
	if (count_150ms<=0)
	{
		count_150ms = 149;
		PORTE=PORTE^(0x10); //toggle PE4
		led_flag=1; //update LEDs and LCD
	}
	
	else
	{
		count_150ms--;
	}
	
	if(((~PIND)&0x01)) //if PD0 is pressed
	{
		deb_count--;
	}
		
	else
	{
		deb_count=49;
		deb_flag=0;
	}
		
	if(deb_count==0) //if pressed for 50 ms
	{
		deb_count = 49;
		deb_flag=1; //pause LEDs and LCD updating
	}
}

int main(void)
{
    led8_init();
    DDRE=0x18;
    DDRD&=0xFE;
    PORTD|=0x01;

    lcd_initialize();
    Timer_initialize();
	
    LCD_CLEAR();
    LCD_WR_Stg("ECE322 Tach Proj");
    LCD_POS(0x40, 1);
    LCD_WR_Stg("RPM  CNT");
	
    sei();

    while(1)
    {		
	if( (led_flag==1) && (deb_flag==0) )
	{
		led8_set(enc_count);							     
		LCD_POS(0x10, 0);
		delay_us(60);
		LCD_WR_Int(rpm,3);
		delay_us(60);
		LCD_WR_Stg("  ");
		delay_us(60);
		LCD_WR_Int(enc_count,2);				 
		led_flag=0;				 
	}   
    }

    return(0);
}



