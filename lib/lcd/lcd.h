#ifndef LCD_DISPLAY
#define LCD_DISPLAY

#include <avr/io.h>
#include <util/delay.h>

void LCD_Command(unsigned char cmnd);
void LCD_Char (unsigned char char_data);	/* LCD data write function */
void LCD_Init (void);			/* LCD Initialize function */
void LCD_String (char *str);		/* Send string to LCD function */
void LCD_String_xy (char row, char pos, char *str);/* Send string to LCD with xy position */
void LCD_Clear();

#endif //LCD_DISPLAY
