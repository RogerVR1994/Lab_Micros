#include "lcd.h"

#include <stm32f10x.h>

#include <string.h>

#include "delay.h"

#define RS 10
#define EN 11
#define D4 12

static void lcdENToggle( void );

void lcdInit( )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH = ( GPIOB->CRH & ~0xFFFFFF00 ) | 0x33333300;
	_delay_ms(20);

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 | 1<<RS | 1<<EN ) ) | 3<<D4;
	lcdENToggle( );
	_delay_ms(5);

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 3<<D4;
	lcdENToggle( );
	_delay_us(100);

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 3<<D4;
	lcdENToggle( );
	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 2<<D4;
	lcdENToggle( );

	lcdCommand(0x28);
	lcdCommand(0x0E);
	lcdCommand(0x06);
	lcdCommand(0x01);
}

void lcdCommand(uint8_t command)
{
	GPIOB->ODR = ( GPIOB->ODR & ~( 1<<RS | 0xF<<D4 ) ) | ( command>>4 )<<D4;
	lcdENToggle( );

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | command<<D4;
	lcdENToggle( );

	if( ( command & 0x02 ) | ( command & 0x01 ) )
		_delay_ms(2);
	else
		_delay_us(50);
}

void lcdData( char data )
{
	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 1<<RS | data>>4<<D4;
	lcdENToggle( );

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | data<<D4;
	lcdENToggle( );

	_delay_us(50);
}

void lcdGotoYX( uint8_t y, uint8_t x )
{
	lcdCommand( y<1 ? 0x80 + x : 0xC0 + x );
}

void lcdPrintLn( char msg[] )
{
	uint8_t i;
	// fancy int. avoids compiler warning when comparing i with strlen()'s uint8_t
	for (i=0;i<strlen(msg) && msg[i] != ' ';i++)
	lcdData(msg[i]);
}

void lcdENToggle( )
{
	GPIOB->ODR |= ( 1<<EN );
	_delay_us(200);
	GPIOB->ODR &= ~( 1<<EN );
}
