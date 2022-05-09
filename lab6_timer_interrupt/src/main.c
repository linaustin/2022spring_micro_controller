#include "stm32l476xx.h"
#include "7seg_function.h"
#include "system_function.h"
#include "button_function.h"
#include "gpio_function.h"
#include "timer_function.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

Timer_Init_Data timer_Data;

void TIM3_IRQHandler();

int sec;

int main(){

	sec = 0;

	init_7seg(&seg_Gpio);
	system_Clock_Config(&sysclk_40Mhz);

	gpio_Init(GPIOA, 5);

	NVIC_SetPriorityGrouping(0b101);
	NVIC_SetPriority(TIM3_IRQn, 0b0000);
	NVIC_EnableIRQ(TIM3_IRQn);

	timer_Data.PSC = (40*1000) - 1;
	timer_Data.ARR = 9;
	timer_Enable(TIM3);
	timer_Init(TIM3, &timer_Data);
	timer_Start_IT(TIM3);




	while(1){
		display_Number_Float2(&seg_Gpio, sec);
	}

	return 0;
}


void  TIM3_IRQHandler(){
	TIM3->SR &= ~TIM_SR_UIF;
	sec++;

	return;
}
