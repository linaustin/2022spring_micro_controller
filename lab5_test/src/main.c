#include "stm32l476xx.h"
#include "music.h"
#include "system_function.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 0,
	.CS = 1,
	.CLK = 2
};

Keypad_TypeDef keypad_Gpio ={
		.out_Gpio[0] = GPIOA,
		.out_Pin[0] = 6,

		.out_Gpio[1] = GPIOA,
		.out_Pin[1] = 7,

		.out_Gpio[2] = GPIOA,
		.out_Pin[2] = 8,

		.out_Gpio[3] = GPIOA,
		.out_Pin[3] = 9,

		.in_Gpio[0] = GPIOB,
		.in_Pin[0] = 3,

		.in_Gpio[1] = GPIOB,
		.in_Pin[1] = 4,

		.in_Gpio[2] = GPIOB,
		.in_Pin[2] = 5,

		.in_Gpio[3] = GPIOB,
		.in_Pin[3] = 6
};

int main(){
	uint16_t keypad_Input;

	system_Clock_Config(&sysclk_40Mhz);
	keypad_Init(&keypad_Gpio);
	init_7seg(&seg_Gpio);
	music_Init();

	while(1){
		keypad_Input = keypad_Read(&keypad_Gpio);
		music_Read_Keypad(keypad_Input, &seg_Gpio);
		delay_Loop(500);

	}

	return 0;
}
