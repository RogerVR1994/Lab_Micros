#include <stm32f10x.h>

#include "delay.h"

int main( )
{
	// Declaración de activaciónde Clock en pines de entrada

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  

	//Declaración de PinC13 como entrada Pull up

	GPIOC->CRH = ( GPIOC->CRH & ~GPIO_CRH_CNF13 & ~GPIO_CRH_MODE13 ) | GPIO_CRH_MODE13;
	
	while(1)
	{	
		//Hacer lectura del estado del pin cada 500 milisegundos
		GPIOC->ODR ^= 1<<13; 

		//Espera de 500 ms
		_delay_ms(500);
		
	}

}
