#include <stm32f10x.h>

void initKeypad( )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH = ( GPIOB->CRH & ~0x0000000F ) | 0x00000004;
	GPIOB->CRL = ( GPIOB->CRL & ~0xFFFF0000 ) | 0x44440000;
}

char getKey( )
{
	char key;
	uint8_t keypad = GPIOB->IDR & 0x100 ? GPIOB->IDR>>4 & 0x0F : 0xFF;

	switch( keypad )
	{
		case 0:
		case 1:
		case 2: 	
			key = keypad+1 + 0x30;
			break;
		case 4:
		case 5:
		case 6: 
			key = keypad + 0x30;
			break;
		case 8:
		case 9:
		case 10: 	
			key = keypad-1 + 0x30;
			break;
		case 12: 	
			key = '*';
			break;
		case 13: 	
			key = '0';
			break;
		case 14: 	
			key = '#';
			break;
		default: 	
			key = ' ';
			break;
	}

	return key;
}