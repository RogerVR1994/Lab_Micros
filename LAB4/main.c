#include <stm32f10x.h>
#include <stdio.h>
#include "delay.h"
#include "lcd.h"
#include "keypad.h"

int main( )
{
	/*
	Función main para imprimir caracteres en LCD

	Se declara variable "keypad" de tipo caracter 
	con permiso de lectura y escritura

	Después se declara el tamaño de la pantalla que 
	es de 2 renglones por 16 columnas

	se inicializa la pantalla mandando a llamar las 
	funciones lcdInit() e initKeypad() de la librería
	lcd.c

	sprintf() es una función que escribe algún  arreglo
	en el renglón screen[i]

	La variable queda de la siguiente manera:

	screen[0]="TE2023 A       B"
	screen[1]="0      1        "

	screen = ["TE2023 A       B";"0      1        ""]

	se posiciona el cursor en la posición 0,0 (inicio de 
	la pantalla) y se escribe el contenido que hay en los
	primero 8 bits del arreglo de caracteres.

	Después se posiciona en la posición 0, 8 se imprime 
	"A      B" debido los espacios no se imprimirá nada entre
	estas dos letras aunque se le inserte en el arreglo.

	En el segundo renglón se hace lo mismo, imprimiendo sólo
	0 y 1

	*/
	__IO char keypad;
	char screen[2][16];
	
	lcdInit();
	initKeypad();
	
	sprintf(screen[0], "TE2023 A      B");
	sprintf(screen[1], "0      1       ");
	
	lcdGotoYX(0,0);
	lcdPrintln(screen[0]);
	
	lcdGotoYX(0,8);
	lcdData(screen[0][8]);
	
	lcdGotoYX(1,0);
	lcdData(screen[1][0]);
	
	lcdGotoYX(1,8);
	lcdData(screen[1][8]);
	
	
	
	
	while(1)
	{	
		lcdGotoYX(1, 15);
		keypad = getKey();
		lcdData(keypad);
		
	}

}
