#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include inbuilt defined Delay header file */
#include <lcd.h>
#include <millis.h>
 
#define LED_BUILTIN (1 << PB5)

int main()
{

	DDRB |= LED_BUILTIN;
	for(;;) {
		PORTB ^= LED_BUILTIN;
		_delay_ms(500);
	}
	return 0;
}
