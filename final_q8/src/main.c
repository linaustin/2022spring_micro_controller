#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"
#include "keypad_function.h"
#include "calculator.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
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

	if(calculator_Init(&keypad_Gpio, &seg_Gpio) != 0){
		return -1;
	}

	int ans;

	while(1){
		calculator_Run(&keypad_Gpio, &seg_Gpio);

		ans = calculator_Cal(command_Buffer);

		if(error_Flag){
				send_7seg_Error(&seg_Gpio);
				delay(1000);
		}
		else if(clean_Flag){

		}
		else{
			while(!(calculator_Interrupt(&keypad_Gpio))){
				display_Number_Float3(&seg_Gpio, ans);
			}
		}

		calculator_Clean(&seg_Gpio);
	}

	return 0;
}
