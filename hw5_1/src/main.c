#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"
#include "keypad_function.h"
#include "timer_function.h"
#include "system_function.h"
#include "stdlib.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

int main(){

	Sysclk_Data **sysclk_Data_Array = malloc(6*sizeof(Sysclk_Data*));
	sysclk_Data_Array[0] = &sysclk_1Mhz;
	sysclk_Data_Array[1] = &sysclk_6Mhz;
	sysclk_Data_Array[2] = &sysclk_10Mhz;
	sysclk_Data_Array[3] = &sysclk_16Mhz;
	sysclk_Data_Array[4] = &sysclk_20Mhz;
	sysclk_Data_Array[5] = &sysclk_40Mhz;

	int system_Clock_Freq[6] = {1, 6, 10, 16, 20, 40};

	gpio_Init(GPIOA, 5);
	button_Init(GPIOC, 13);
	init_7seg(&seg_Gpio);

	int button_Flag = 0;
	int system_Clock_Mode = 0;
	int led_State = 0;

	while(1){
		button_Flag = button_Read_RisingEdge(GPIOC, 13, 0);

		if(button_Flag){
			system_Clock_Mode++;

			if(system_Clock_Mode > 5){
				system_Clock_Mode = 0;
			}

			system_Clock_Config(sysclk_Data_Array[system_Clock_Mode]);
		}

		display_Number_Float2(&seg_Gpio, system_Clock_Freq[system_Clock_Mode]*100);

		if(led_State){
			gpio_Set(GPIOA, 5);
		}
		else{
			gpio_Reset(GPIOA, 5);
		}

		led_State = 1 - led_State;
	}

	free(sysclk_Data_Array);

	return 0;
}
