/*
Librería para compilación en C
de la tarjeta ST32f106
*/
#include <stm32f10x.h>

void initKeypad( )
{

	/*
	Declaración de función para inicializar
	keypad.
	
	La función se encarga de inicializar el
	clock del GPIOB y
	declarar los pines  [4:8] como entrada

	*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH = ( GPIOB->CRH & ~0x0000000F ) | 0x00000004;
	GPIOB->CRL = ( GPIOB->CRL & ~0xFFFF0000 ) | 0x44440000;
}

char getKey( )
{

	/*
	Función para leer que botón está siento oprimido
	por el usuario.

	Se declara una variable key que seré el botón orpimido.

	Si la variable keypad lee el pin 8 recorre el valor de IDR
	4 lugares y se forzan a 1 los pines [0:3]. 
	De lo contrario se forzan los pines [1:7].

	

	Los valores que puede tomar keypad van del 0x0 al 0xF

	Si la variable keypad es igual a 0,1 o 2 la variable key
	toma el valor de keypad +1 +0x30.

	Si la variable keypad es igual a 4,5 o 6 la variable key
	toma el valor de keypad  +0x30.


	Si la variable keypad es igual a 8,9 o 10 la variable key
	toma el valor de keypad -1 +0x30.


	Si la variable keypad es igual a 12 la variable key
	toma el valor de '*'.
	Si la variable keypad es igual a 13 la variable key
	toma el valor de '0'.
	Si la variable keypad es igual a 14 la variable key
	toma el valor de '#'.

	La razón de la ausencia de valores 3, 7 y 11 se debe a que 
	el keypad tiene una dimensión de 4x3

	al final la función regresa el valor del botón presionado
	*/
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