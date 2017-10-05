#include <stm32f10x.h>

#include <stdio.h>

#include "delay.h"
#include "lcd.h"
#include "keypad.h"

int main( )
{
	RCC->APB2ENR |= ( 0x1<<3 );
	GPIOB->CRH &=0xFFFFFFFF;
	GPIOB->CRH |=0x00000400;
	//GPIOB->CRH = ( GPIOB->CRH & ~ ( 0xF<<(4*(9-8)) ) ) | ( 0x4<<(4*(9-8)) );
	
	__IO int contador=0;
	__IO char keypad;
	__IO int i,j,k,l,a,b,c,d;  //Define control variables
	char screen[2][16];

	lcdInit( );
	initKeypad( );
	
	reset_variable:
	contador=0;
	sprintf( screen[0], "00:00" );

	lcdGotoYX( 0, 0 );
	lcdPrintLn( screen[0] );
	lcdGotoYX( 0, 0 );
	while(1)
	{
		keypad = getKey( );
		if(keypad == '#'){
			if (contador<4){
			contador++;}
			else{
				lcdGotoYX( 0, 0 );
				contador=0;
			}
			do{
				keypad = getKey();
			}while(keypad == '#'); 
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
				keypad = getKey(); //Read and save value in keypad
			}while(keypad == '*'); //Wait while '*' still being pressed
			lcdGotoYX( 0, contador ); 
			}
		
			else if (keypad!=' ' && keypad!='#' && keypad!='*' && contador!=2){ //If something different to ''#,'*' was pressed in keypad, and LCD position is different to 0,2
			screen[0][contador]=keypad; //Read value in keypad and save in array position 0,var
			lcdGotoYX( 0, 0 ); //Go to position 0,0 in LCD
			lcdPrintLn(screen[0]); //Print array value in position 0
			lcdGotoYX( 0, contador ); //Go to position 0,var in LCD
			do{
				keypad = getKey(); //Read and save value in keypad
			}while(keypad!=' ' && keypad!='#' && keypad!='*' && contador!=2); //Wait while something diferent to '#','*'' still being pressed
		}

			
		if(!(GPIOB->IDR & 0x200) & (((screen[0][0])!='0')|((screen[0][1])!='0')|((screen[0][2])!='0')|((screen[0][3])!='0'))){ //If some number was added to the LCD and start button was pressed, start counting
			a=screen[0][0]-48; //Save value position 0,0 in a, substract 48 because 0 in ASCII code
			b=screen[0][1]-48; //Save value position 0,1 in b, substract 48 because 0 in ASCII code
			c=screen[0][3]-48; //Save value position 0,3 in c, substract 48 because 0 in ASCII code
			d=screen[0][4]-48; //Save value position 0,4 in d, substract 48 because 0 in ASCII code
			for(i=a;i>=0;i--){ //Cicle for decrementing count of the first value in "00:00" starting on a value
				screen[0][0]=i+'0'; //Write value i in array position 0,0, add '0' because of ASCCI code
				for(j=b;j>=0;j--){ //Cicle for decrementing count of the second value in "00:00" starting on b value
					screen[0][1]=j+'0'; //Write value j in array position 0,1, add '0' because of ASCCI code
					for(k=c;k>=0;k--){ //Cicle for decrementing count of the third value in "00:00" starting on c value
						screen[0][3]=k+'0'; //Write value i in array position 0,3, add '0' because of ASCCI code
						for(l=d;l>=0;l--){ //Cicle for decrementing count of the fourth value in "00:00" starting on d value
							_delay_ms(1000); //Wait 1 second
							screen[0][4]=l+'0'; //Write value i in array position 0,4, add '0' because of ASCCI code
							lcdGotoYX( 0, 0 ); //Go to position 0,0 in LCD
							lcdPrintLn(screen[0]); //Print "time" value in position 0
							lcdGotoYX( 0, 0 ); //Go to position 0,0 in LCD					
							if(!(GPIOB->IDR & 0x200)){ //Out if button was pressed
								goto reset_variable; //Go to label salir
							}
						}
						d=9; //Value 9 to fourth value in "00:00" when each cicle is over
					}
					c=5; //Value 5 to third value in "00:00" when each cicle is over 
				}
			}
			lcdGotoYX( 0, 0 ); //Go to position 0,0 in LCD
			contador=0; //Assign 0 to variable var
		}
	}
}
