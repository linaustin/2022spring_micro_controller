#include "stm32l476xx.h"
#include "7seg_function.h"
#include "keypad_IT_function.h"
#include "system_function.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

int main(){

	init_7seg(&seg_Gpio);

	int keypad_Input;

	if(keypad_IT_Init()){
		return -1;
	}

	while(1){
		keypad_Input = keypad_Read();
		if(keypad_Input  != -1){
			display_Number_Float2(&seg_Gpio, keypad_Input*100);
		}
	}

	return 0;
}

