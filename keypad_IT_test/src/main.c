#include "stm32l476xx.h"
#include "adc_function.h"
#include "timer_function.h"
#include "7seg_function.h"
#include "button_function.h"
#include "keypad_IT_function.h"

Seg_TypeDef seg_Gpio= {
		.gpio = GPIOC,
		.DIN = 5,
		.CS = 6,
		.CLK = 8
};

int main(){

	system_Clock_Config(&sysclk_40Mhz);

	if(init_7seg(&seg_Gpio) != 0){
		return -1;
	}

	keypad_IT_Init();

	int key = 0;

	while(1){
		key = keypad_Read();

		if(key != -1){
			display_Number_Float2(&seg_Gpio, key*100);
			delay_Loop(10000);
		}
		else{
			for(int i = 0; i < 8; i++){
				send_7seg_Char(&seg_Gpio, i, ' ');
			}
		}
	}

	return 0;
}


