/* Code provided by Dr. Aleksander Malinowski for ECE 205 */

#include "bios_timer_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define xtal 16000000L

void Timer_initialize ()
{
    TCNT3=0x00;
    TCCR3A = 0x00;           
    TCCR3B |= (1<<CS32)|(1<<CS31)|(1<<CS30);
    TCCR2 |= (1<<WGM21)|(1<<CS21)|(1<< CS20); 
    OCR2=249;
    TIMSK |= (1<<OCIE2); 
}