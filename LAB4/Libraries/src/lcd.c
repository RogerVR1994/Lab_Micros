//Se incluyen las librerías para cada tarea
//"lcd.h" se encarga de controlar todas las 
//operaciones de la pantalla LCD

//stm32f10x.h da los comandos básicos para
//la compilación de la tarjeta

/*string.h permite el manejo de arreglos de
caracteres

delay.h se encarga de hacer pausas en múltiplos
de 1 ms

*/

#include "lcd.h" 

#include <stm32f10x.h>

#include <string.h>

#include "delay.h"

/*
Declaración de pines de LCD

RS se encarga del hacer la selección
de registros de la pantalla
0: para registro de instrucciones
1: para registro de datos

EN habilita el uso de la pantalla

D4 es uno de los pines de escritura de la 
pantalla
*/

#define RS 10
#define EN 11
#define D4 12

static void lcdENToggle( void );
//Declaración de función lcdENToggle


/*
Declaración de función lcdInit()


*/
void lcdInit( )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH = ( GPIOB->CRH & ~0xFFFFFF00 ) | 0x33333300;
	//Se inicializan los pines [10:15] 
	//para uso de la pantalla
	_delay_ms(20);
	//Espera de 20ms después de que Vcc alcanza 4.5V
	//Esto se hacae por consideraciones del fabricante

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 | 1<<RS | 1<<EN ) ) | 3<<D4;
	//Se escribe un F recorrido 12 lugares,
	//lo que significa enviar 1111 a los 
	//pines [12:15]  
	//un 1 en el pin 10
	
	//Debido a que está negada la operación
	//todos los unos se vuelven 0s y viceversa
	// el registro B quedaría como:
	//0x00000011111111

	//Después un 3 recorrido 12 lugares sin modificar,
	//los otros bits del registro,
	// o 1 a los pines 12 y 13
	//El registro B final tendría la forma:
	//0x00110011111111

	//Esto declara que la longitud de la interfaz 
	//es de 4 bits
	
	lcdENToggle( );
	//Reseteo de instrucción
	_delay_ms(5);
	//Espera de 5 ms (sugerencia del fabricante)

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 3<<D4;
	//Después un 3 recorrido 12 lugares sin modificar,
	//los otros bits del registro,
	// o 1 a los pines 12 y 13
	//El registro B final tendría la forma:
	//0x00110011111111

	//Esto declara que la longitud de la interfaz 
	//es de 4 bits
	lcdENToggle( );
	//Reseteo de instrucción
	_delay_us(100);
	//delay de 100 ms

	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 3<<D4;
	lcdENToggle( );
	GPIOB->ODR = ( GPIOB->ODR & ~( 0xF<<D4 ) ) | 2<<D4;

	//Se envía un 1 al pin 13
	lcdENToggle( );

	//Las instrucciones pasadas son para hacer
	//la declaraíón de una interfaz de 4 bits

	lcdCommand(0x28);
	lcdCommand(0x0E);
	lcdCommand(0x06);
	lcdCommand(0x01);
}

void lcdCommand(uint8_t command)
{
	/*
	Función para recibir comandos para la LCD
	Se escriben en formato hexadecimal.

	la función comienza forzando el registro a la
	forma: 0x00000011111111

	hace el reseto de instrucción, 

	y después envía el comando en hexadecimal a los pines [12:15]

	Si el comando es = a 0x02 0 0x01 hace una pausa
	de 2ms

	Con cualquier otro comando hace una pausa de 50ms
	*/
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

	/*
	Instrucción para posicionar el cursor en las coordenadas 
	x, y.
	Si la coordenada y es menor a 1 utiliza el comando
	0x80 + x
	de lo contrario utiliza el comando 0xc0+x
	*/
	lcdCommand( y<1 ? 0x80 + x : 0xC0 + x );
}

void lcdPrintLn( char msg[] )
{

	/*
	Función para escribir cadena de caracteres en 
	pantalla LCD.

	se hace un recorrimiento desde el principio
	de la cadena, hasta que se llega a un 
	número del tamañano de la cadena y el caracter 
	es diferente de un espacio. En C todos los arreglos
	de caracteres terminan con 0

	Se escribe el caracter en el que se encuentra el 
	loop en la pantalla
	*/
	uint8_t i;
	// fancy int. avoids compiler warning when comparing i with strlen()'s uint8_t
	for (i=0;i<strlen(msg) && msg[i] != ' ';i++)
	lcdData(msg[i]);
}

void lcdENToggle( )
{

/*
Función para indicar nueva instrucción
Debe ser utilizada entre cada instrucción
enviada a la LCD.

Se hace mandando un pulso de 200ms al 
pin 11
*/
	GPIOB->ODR |= ( 1<<EN );
	_delay_us(200);
	GPIOB->ODR &= ~( 1<<EN );
}

