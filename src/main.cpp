#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include inbuilt defined Delay header file */
#include <lcd.h>
#include <millis.h>
 
#define LED_BUILTIN (1 << PB5)

int main()
{

	LCD_Init();
	LCD_String("Hello World!");
	LCD_Command(0xC0);
	LCD_String("from Atmega!");
	while(1);
	return 0;
}
