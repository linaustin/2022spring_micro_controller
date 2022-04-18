#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"
#include "keypad_function.h"


Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 3,
	.CS = 4,
	.CLK = 5
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

int main(){

	init_7seg(&seg_Gpio);
	init_Keypad(&keypad_Gpio);

	uint16_t keypad_Input;

	display_Number(&seg_Gpio, 1000*2.5);

	while(1){
		keypad_Input = read_Keypad(&keypad_Gpio);

		for(int i = 0; i < 16; i++){
			if((keypad_Input >> i) & 0x01){
				display_Number(&seg_Gpio, i*1000);
				break;
			}
		}

   }

	return 0;
}
