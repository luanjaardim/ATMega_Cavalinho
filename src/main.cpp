#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define ADC_0 PB1
#define ADC_1 PB2

#define LCD_Dir  DDRD			/* Define LCD data port direction */
#define LCD_Port PORTD			/* Define LCD data port */
#define RS PD2				/* Define Register Select pin */
#define RW PD3				/* Define Read or Write mode pin */
#define EN PB0 				/* Define Enable signal pin */

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	LCD_Port &= ~(1<<RS);		/* RS=0, command reg. */
	PORTB |= (1<<EN);		/* Enable pulse */
	_delay_us(1);
	PORTB &= ~(1<<EN);
	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	PORTB |= (1<<EN);
	_delay_us(1);
	PORTB &= ~(1<<EN);
	_delay_us(200);
}

void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	LCD_Port |= (1<<RS);		/* RS=1, data reg. */
	PORTB |= (1<<EN);
	_delay_us(1);
	PORTB &= ~(1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
	PORTB |= (1<<EN);
	_delay_us(1);
	PORTB &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Init(void)			/* LCD Initialize function */
{
	LCD_Dir = 0xF0;			/* Make LCD port direction DDRD 4-7 output */
	LCD_Dir |= (1 << RS | 1 << RW);	/* Make LCD RS and RW pin output */
	LCD_Port &= ~(1<<RW); /* Enters Write mode */
	DDRB |= (1 << EN);	/* Make EN pin output */
	_delay_ms(200);			/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);              /* Display on cursor off*/
	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              /* Clear display screen*/
	_delay_ms(2);
}

void LCD_String(char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy(char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}

volatile unsigned long timer1_micros;
//NOTE: A unsigned long holds values from 1 to 4,294,967,295 (2^32 - 1). It will roll over to 0 after reaching its maximum value.

ISR(TIMER1_COMPA_vect)
{
  timer1_micros++;
}

void init_micros(unsigned long f_cpu)
{
  unsigned long ctc_match_overflow;

  ctc_match_overflow = ((f_cpu / 1000000) / 8); //when timer1 is this value, 1ms has passed

  // (Set timer to clear when matching ctc_match_overflow) | (Set clock divisor to 8)
  TCCR1B |= (1 << WGM12) | (1 << CS11);

  // high byte first, then low byte
  OCR1AH = (ctc_match_overflow >> 8);
  OCR1AL = ctc_match_overflow;

  // Enable the compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  //REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH sei(); !!!
}

unsigned long micros(void)
{
  unsigned long micros_return;

  // Ensure this cannot be disrupted
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    micros_return = timer1_micros;
  }
  return micros_return;
}
 
int main()
{
  //LCD_Init();
	//LCD_String("Hello World,");
	//LCD_Command(0xC0);
	//LCD_String("from Atmega!");
  	//while(1);
	LCD_Init();			/* Initialization of LCD*/
  	init_micros(F_CPU);
  	sei();
    //	LCD_String("Hello");
	//_delay_ms(100);
  	unsigned long tempo;
  	char str[10];
  	DDRB &= ~(1<<ADC_0 | 1<<ADC_1);
  while(1){
      if(PINB & (1<<ADC_0)) {
      	tempo = micros();
        while(!(PINB & (1<<ADC_1)));
        tempo = micros() - tempo;
        itoa(tempo, str, 10);
        LCD_String("Tempo:");
        LCD_Command(0xC0);
        LCD_String((char *) str);
        _delay_ms(1000);
        LCD_Clear();
      }
      if(PINB & (1<<ADC_1)) {
      	tempo = micros();
        while(!(PINB & (1<<ADC_0)));
        tempo = micros() - tempo;
        itoa(tempo, str, 10);
        LCD_String("Tempo:");
        LCD_Command(0xC0);
        LCD_String((char *) str);
        _delay_ms(1000);
        LCD_Clear();
      }
      
  	//LCD_Clear();
    //tempo = millis();
    //itoa(tempo, str, 10);
    //LCD_String((char *) str);
    //_delay_ms(1000);
    //LCD_Command(0xC0);
    //tempo = millis();
    //itoa(tempo, str, 10);
    //LCD_String((char *) str);
    //_delay_ms(10000);
  }
}

// #include <avr/io.h>			/* Include AVR std. library file */
// #include <util/delay.h>			/* Include Delay header file */

// #define LCD_Dir  DDRB			/* Define LCD data port direction */
// #define LCD_Port PORTB			/* Define LCD data port */
// #define RS PB0				/* Define Register Select pin */
// #define EN PB1 				/* Define Enable signal pin */

// void LCD_Command( unsigned char cmnd )
// {
// 	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
// 	LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
// 	LCD_Port |= (1<<EN);		/* Enable pulse */
// 	_delay_us(1);
// 	LCD_Port &= ~ (1<<EN);

// 	_delay_us(200);

// 	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
// 	LCD_Port |= (1<<EN);
// 	_delay_us(1);
// 	LCD_Port &= ~ (1<<EN);
// 	_delay_ms(2);
// }

// void LCD_Char( unsigned char data )
// {
// 	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
// 	LCD_Port |= (1<<RS);		/* RS=1, data reg. */
// 	LCD_Port|= (1<<EN);
// 	_delay_us(1);
// 	LCD_Port &= ~ (1<<EN);

// 	_delay_us(200);

// 	LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
// 	LCD_Port |= (1<<EN);
// 	_delay_us(1);
// 	LCD_Port &= ~ (1<<EN);
// 	_delay_ms(2);
// }

// void LCD_Init (void)			/* LCD Initialize function */
// {
// 	LCD_Dir = 0xFF;			/* Make LCD port direction as o/p */
// 	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
// 	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
// 	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
// 	LCD_Command(0x0c);              /* Display on cursor off*/
// 	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
// 	LCD_Command(0x01);              /* Clear display screen*/
// 	_delay_ms(2);
// }

// void LCD_String (char *str)		/* Send string to LCD function */
// {
// 	int i;
// 	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
// 	{
// 		LCD_Char (str[i]);
// 	}
// }

// void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
// {
// 	if (row == 0 && pos<16)
// 	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
// 	else if (row == 1 && pos<16)
// 	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
// 	LCD_String(str);		/* Call LCD string function */
// }

// void LCD_Clear()
// {
// 	LCD_Command (0x01);		/* Clear display */
// 	_delay_ms(2);
// 	LCD_Command (0x80);		/* Cursor at home position */
// }
 
// int main()
// {
// 	LCD_Init();			/* Initialization of LCD*/

// 	LCD_String("ElectronicWINGS");	/* Write string on 1st line of LCD*/
// 	LCD_Command(0xC0);		/* Go to 2nd line*/
// 	LCD_String("Hello World");	/* Write string on 2nd line*/
// 	while(1);
// }