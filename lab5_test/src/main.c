#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"
#include "keypad_function.h"
#include "timer_function.h"
#include "system_function.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

Keypad_TypeDef keypad_Gpio = {
		.out_Gpio[0] = GPIOB,
		.out_Pin[0] = 6,

		.out_Gpio[1] = GPIOA,
		.out_Pin[1] = 7,

		.out_Gpio[2] = GPIOA,
		.out_Pin[2] = 6,

		.out_Gpio[3] = GPIOA,
		.out_Pin[3] = 5,

		.in_Gpio[0] = GPIOB,
		.in_Pin[0] = 10,

		.in_Gpio[1] = GPIOA,
		.in_Pin[1] = 8,

		.in_Gpio[2] = GPIOA,
		.in_Pin[2] = 9,

		.in_Gpio[3] = GPIOC,
		.in_Pin[3] = 7
};

uint32_t count = 0;

int main(){

	system_Clock_Config(&sysclk_1Mhz);

	Timer_Init_Data clock = {
		.PSC = 10000 - 1,
		.ARR = 1000 - 1
	};

	timer_Enable(TIM2);
	timer_Init(TIM2, &clock);
	timer_Start(TIM2);

	while(1){

		count = TIM2->CNT;
	}

	return 0;
}
