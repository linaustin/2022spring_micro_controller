#include "stm32l476xx.h"
#include "7seg_function.h"
#include "system_function.h"
#include "gpio_function.h"
#include "keypad_IT_function.h"


Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

int main(){


	return 0;
}


