#include "stm32l476xx.h"
#include "gpio_function.h"
#include "7seg_function.h"
#include "timer_function.h"
#include "system_function.h"


Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 5,
	.CS = 6,
	.CLK = 8
};
/*
Keypad_TypeDef keypad_Gpio ={
		.out_Gpio[0] = GPIOA,
		.out_Pin[0] = 5,

		.out_Gpio[1] = GPIOA,
		.out_Pin[1] = 6,

		.out_Gpio[2] = GPIOA,
		.out_Pin[2] = 7,

		.out_Gpio[3] = GPIOA,
		.out_Pin[3] = 8,

		.in_Gpio[0] = GPIOB,
		.in_Pin[0] = 3,

		.in_Gpio[1] = GPIOB,
		.in_Pin[1] = 4,

		.in_Gpio[2] = GPIOB,
		.in_Pin[2] = 5,

		.in_Gpio[3] = GPIOB,
		.in_Pin[3] = 6
};
*/

int main(){
	system_Clock_Config(&sysclk_40Mhz);

	init_7seg(&seg_Gpio);

	gpio_Init(GPIOC, 0);//led 1
	gpio_Init(GPIOC, 1);//led 2

	gpio_Set(GPIOC, 0);//turn off led1
	gpio_Set(GPIOC,1);// turn off led2

	Timer_Init_Data tim_Data = {
			.PSC = (40 * 1000) - 1,
			.ARR = 5000 - 1
	};

	timer_Enable(TIM2);//
	timer_Init(TIM2, &tim_Data);

	gpio_Reset(GPIOC, 0);
	TIM2->SR = 0;
	timer_Start(TIM2);

	while(!(TIM2->SR & TIM_SR_UIF)){
		display_Number_Float2(&seg_Gpio, (TIM2->CNT/10));
		delay_Loop(1000);
	}
	display_Number_Float2(&seg_Gpio, 500);

	timer_Stop(TIM2);

	gpio_Set(GPIOC, 0);
	gpio_Reset(GPIOC, 1);

	return 0;
}
