#include "stm32l476xx.h"
#include "music.h"
#include "system_function.h"
#include <stdio.h>

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 3,
	.CS = 4,
	.CLK = 5
};

Keypad_TypeDef keypad_Gpio ={
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

int main(){

	system_Clock_Config(&sysclk_40Mhz);
	keypad_Init(&keypad_Gpio);
	init_7seg(&seg_Gpio);
	music_Init();

	int input_Note[7] = {1,1,5,5,6,6,5};

	while(1){
		for(int i = 0; i < 7; i++){
			music_Play(input_Note[i]);
			delay_Loop(2000);
			timer_Channel_Stop(TIM2, 1);
			delay_Loop(1000);
		}

	}
	return 0;
}
