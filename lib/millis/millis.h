#ifndef _MILLIS_H_
#define _MILLIS_H_

#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect);
void init_millis(unsigned long f_cpu);
unsigned long millis (void);

#endif