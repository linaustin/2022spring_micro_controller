#include "stm32l476xx.h"
#include "7seg_function.h"
#include "system_function.h"
#include "button_function.h"
#include "gpio_function.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

int systick_Reload_Value[3] = {2500000 - 1, 500000 - 1, 1500000 - 1};
int display_Number[3] = {50, 10, 30};

void SysTick_Handler();

int main(){

	init_7seg(&seg_Gpio);
	button_Init(GPIOC, 13);
	system_Clock_Config(&sysclk_10Mhz);
	gpio_Init(GPIOA, 5);

	int button_Flag = 0;
	int systick_Freq = 0;

	system_Tick_Start_IT(systick_Reload_Value[0]);
	display_Number_Float2(&seg_Gpio, display_Number[0]);

	while(1){
		button_Flag = button_Read_RisingEdge(GPIOC, 13, 0);

		if(button_Flag){
			systick_Freq++;

			if(systick_Freq == 3){
				systick_Freq = 0;
			}

			system_Tick_Start_IT(systick_Reload_Value[systick_Freq]);
			display_Number_Float2(&seg_Gpio, display_Number[systick_Freq]);
		}
	}

	return 0;
}

void SysTick_Handler(){
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;

		gpio_Toggle(GPIOA, 5);

	}

	return;
}
