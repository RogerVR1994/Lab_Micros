#include <stm32f10x.h>
#include "delay.h"

int main(){
	uint16_t nextState = 0;
	RCC->APB2ENR |= (1<<3);
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &= 0xFF0000FF;
	GPIOA->CRL |= 0x00333300;
	GPIOB->CRH &= 0xFFFFFFFF;
	
	while(1){
		
		switch(nextState){
			case 0:
				GPIOA->ODR = 0x00;
				if (GPIOB->IDR & 0x2000){
					nextState = 4;
				}
				else
					nextState = 1;
				break;
			case 1:
				GPIOA->ODR = 0x14;
				if (GPIOB->IDR & 0x2000){
					nextState = 0;
				}
				else
					nextState = 2;
				break;
			case 2:
				GPIOA->ODR = 0x18;
				if (GPIOB->IDR & 0x2000){
					nextState = 1;
				}
				else
					nextState = 3;
				break;
			case 3:
				GPIOA->ODR = 0x24;
				if (GPIOB->IDR & 0x2000){
					nextState = 2;
				}
				else
					nextState = 4;
				break;
			case 4:
				GPIOA->ODR = 0x28;
				if (GPIOB->IDR & 0x2000){
					nextState = 3;
				}
				else
					nextState = 0;
				break;
		}
		_delay_ms(1300);
		
	}
}