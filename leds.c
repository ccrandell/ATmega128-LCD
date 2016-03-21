/* Code provided by Dr. Aleksander Malinowski for ECE 205 */
//* bios_led8.c *

#include "bios_led8.h"
#include <avr/io.h>

void led8_init (void)
{
    // set all pins on PORTA for output
    DDRB  = 0xFF;   // 0b11111111
}

void led8_set ( uint8_t number )
{
    PORTB=number;
}