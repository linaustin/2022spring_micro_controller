#include "stm32l476xx.h"
#include "7seg_function.h"
#include "system_function.h"
#include "gpio_function.h"
#include "keypad_IT_function.h"


Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

void EXTI15_10_IRQHandler();

int main(){

	init_7seg(&seg_Gpio);
	gpio_Init(GPIOB, 6);

	GPIOB->MODER &= ~(0b11 << (2*6));
	GPIOB->MODER |= (0b01 << (2*6));

	GPIOB->OSPEEDR &= ~(0b11 << (2*6));
	GPIOB->OSPEEDR |= (0b11 <<(2*6));

	GPIOB->OTYPER &= ~(0b01 << 6);

	gpio_Set(GPIOB, 6);



	gpio_Init(GPIOB, 10);

	GPIOB->MODER &= ~(0b11 << (2*10));
	GPIOB->MODER |= (0b00 << (2*10));

	GPIOB->PUPDR &= ~(0b11 << (2*10));
	GPIOB->PUPDR |= (0b10 << (2*10));

	gpio_Reset(GPIOB, 10);

	keypad_EXTI_config(GPIOB, 10);

	NVIC_SetPriorityGrouping(0b101);

	NVIC_SetPriority(EXTI15_10_IRQn, 0b0000);
	NVIC_EnableIRQ(EXTI15_10_IRQn);



	while(1){
		display_Number_Float2(&seg_Gpio, 0);
	}

	return 0;
}

void EXTI15_10_IRQHandler(){
	if((EXTI->PR1 >> 10) & 0b01){
		EXTI->PR1 |= (0b01 << 10);

		display_Number_Float2(&seg_Gpio, 100);
		delay_Loop(500);
	}

	return;
}
