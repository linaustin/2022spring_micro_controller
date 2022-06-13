#include "stm32l476xx.h"
#include "gpio_function.h"
#include "7seg_function.h"
#include "keypad_function.h"

/*
Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 5,
	.CS = 6,
	.CLK = 8
};

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

//my student id is 0811074
// target number is 4 >> 0b0100

int main(){
	//init gpio pins
	gpio_Init(GPIOC, 9);
	gpio_Init(GPIOC, 10);
	gpio_Init(GPIOC, 11);
	gpio_Init(GPIOC, 12);
	//setting led state
	gpio_Set(GPIOC, 9); //0
	gpio_Set(GPIOC, 10); //0
	gpio_ReSet(GPIOC, 11); //1
	gpio_Set(GPIOC, 12); //0

	return 0;
}
