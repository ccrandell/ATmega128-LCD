/* Code provided by Dr. Aleksander Malinowski for ECE 205 */
//* Timer1 interrupt-based library - header file *


#ifndef BIOS_Timer_INT_H_
#define BIOS_Timer_INT_H_
#include <stdint.h>

#ifndef _PRESCALER_DEFS
#define _PRESCALER_DEFS
    #define timer_prescale_1    (1)
    #define timer_prescale_8    (2)
    #define timer_prescale_64   (3)
    #define timer_prescale_256  (4)
    #define timer_prescale_1024 (5)
#endif


void    Timer_initialize   ( );
//void    Timer2_initialize    ();

#endif /* BIOS_Timer1_INT_H_ */
