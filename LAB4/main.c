#include <stm32f10x.h>
#include <stdio.h>
#include "delay.h"
#include "lcd.h"
#include "keypad.h"

int main( )
{
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
