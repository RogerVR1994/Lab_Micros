#include <stm32f10x.h>
#include <stdio.h>
#include "delay.h"
#include "lcd.h"
#include "keypad.h"

/*
Código para cronómetro utilizando pantalla LCD y keypad
Primeros pasos: importar librerías para LCD, keypad, delay, 
compilación para tarjeta.

Es necesario inicializar el clock para el GPIOB y declarar 
como entrada el pin 10.

Configurar pantalla e inicializarla

reset_variable: es una etiqueta que una vez que es llamada
interrumpe las demás acciones que este realizando el microcontrolador
y regresa a la posición de la etiqueta. Lo mismo sucede con err:

guardar en variable screen[] el valor de inicio "00:00".
Se utiliza la función sprintf( char screen[0], ""00:00)

lcdGotoYX(y, x) permite posicionar el cursor en cierta 
posición deseada de la pantalla.

lcdPrintLn(char) recibe un arreglo de caracteres y lo imprime
en formato ASCII en la posición dictada por el programa.

Una vez dentro del Loop:
Se obtiene la lectura de los botones en el keypad.

Se utiliza la tecla # para moverse a la derecha y *
para moverse a la izquierda.

El algoritmo para esta tarea se basa en revisar la posición en 
la que se encuentra el cursor. Si se está moviendo a la
derecha y rebasa la posición 4, se regresa el cursor a la posición
inicial.
Por el contrario, si se mueve a la iquierda y rebasa la posición 0
se regresa el cursor a la posición final (4).

Si el keypad recibe cualquier tecla númerica, se escribe en el cronómetro.
Esta tarea no se cumple si el cursor se encuentra en la posición de ":".
Para las posiciónes decimales de los segundos y los minutos sólo acepta
valores entre 0 y 5, debido a que no se puede representar el tiempo como "61 segundos", 
"90 minutos", etc.

Cada vez que se hace una modiciación en el display, se vuelve a imprimir todo el dato.
Sin embargo se hace tan rápido que es imperciptible para el usuario.

Cuando se presiona el botón de Start/Stop y el valor del
cronómetro es diferente de 00:00, se inicia la cuenta regresiva con respecto al
valor establecido por el usuario.

Para hacer la conversión de la cadena de caracteres del tiempo, 
a tipo de dato numérico, es necesario realizar una operación para convertir
ASCII a enteros. Esto se logra restando 48 al valor del código
ASCII, debido a que el valor del '0' es 48. De esta manera: 

el número 9 en ASCII es 57, por lo que: 57 - 48 = 9
en caso de se inserte un número mayor a 5 en las posiciones
de las decenas, el microcontrolador no hace nada y espera otro dato.

Una vez que se tiene el valor de los cuatro caracteres del tiempo, 
se ejecutan loops for hasta que todos los números lleguen a 0.

Las posiciones de las unidades al llegar a 0 se reinician en 9.
Las posiciones de las decenas al llegar a 0 se reinician en 5.


*/

int main( )
{
	RCC->APB2ENR |= ( 0x1<<3 );
	GPIOB->CRH &=0xFFFFFFFF;
	GPIOB->CRH |=0x00000400;
	
	int contador=0;
	char keypad;
	int i,j,k,l,a,b,c,d;  
	int reset =0;
	char screen[2][16];

	lcdInit( );
	initKeypad( );
	
	reset_variable: contador=0;
	sprintf( screen[0], "00:00" );

	lcdGotoYX( 0, 0 );
	lcdPrintLn( screen[0] );
	lcdGotoYX( 0, 0 );
	err: _delay_ms(100);
	while(1)
	{
		keypad = getKey( );
		if(keypad == '#'){
			if (contador<4){
				contador++;
			}
			else{
				lcdGotoYX( 0, 0 );
				contador=0;
			}
			do{
				keypad = getKey();
			}
			while(keypad == '#'); 
				lcdGotoYX( 0, contador );
		}
		else if(keypad == '*'){
			if (contador>0){
				contador--;}
			else{
				lcdGotoYX( 0, 0 );
				contador=4;
			}
			do{
				keypad = getKey(); 
			}
			while(keypad == '*'); 
				lcdGotoYX( 0, contador ); 
		}
		
		else if (keypad!=' ' && keypad!='#' && keypad!='*' && contador!=2){
				
			if ((contador == 0 || contador ==3)&&(keypad=='6' || keypad == '7' || keypad == '8' || keypad == '9')){
				goto err;
			}
			screen[0][contador]=keypad; 
			lcdGotoYX( 0, 0 ); 
			lcdPrintLn(screen[0]);
			lcdGotoYX( 0, contador );
			do{
				keypad = getKey(); 
			}
			while(keypad!=' ' && keypad!='#' && keypad!='*' && contador!=2); /
		}

			
		if(!(GPIOB->IDR & 0x200) & (((screen[0][0])!='0')|((screen[0][1])!='0')|((screen[0][2])!='0')|((screen[0][3])!='0'))){ /was pressed, start counting
			reset = 0;
			a=screen[0][0]-48; 
			b=screen[0][1]-48; 
			c=screen[0][3]-48; 
			d=screen[0][4]-48; 
			for(i=a;i>=0;i--){ 

				screen[0][0]=i+'0'; 
				
				for(j=b;j>=0;j--){ 
				
					screen[0][1]=j+'0'; 
				
					for(k=c;k>=0;k--){ 
				
						screen[0][3]=k+'0'; 
				
						for(l=d;l>=0;l--){ 
				
							_delay_ms(1000); 
							screen[0][4]=l+'0'; 
							lcdGotoYX( 0, 0 );
							lcdPrintLn(screen[0]); 
							lcdGotoYX( 0, 0 ); 				
				
							if(!(GPIOB->IDR & 0x200)){ 
				
								goto reset_variable;
								
							}
						}
				
						d=9; 
					}

					c=5; 
				}

				b=9;
			}

			a=5;
			lcdGotoYX( 0, 0 ); 
			contador=0; 
		}
	}
}

