#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"
#include "keypad_function.h"

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

void update_7seg(Seg_TypeDef* seg_Data, int* display_Data, uint16_t keypad_Input);

int main(){

	if(init_7seg(&seg_Gpio) != 0 || init_Keypad(&keypad_Gpio) != 0){
		return -1;
	}

	uint16_t keypad_Input = 0;
	int display_Data[8];

	for(int i = 0; i < 8; i++){
		display_Data[i] = -1;
	}

	while(1){
		keypad_Input = read_Keypad(&keypad_Gpio);

		if(keypad_Input == 0x4000){
			for(int i = 0; i < 8; i++){
				display_Data[i] = -1;
				send_7seg_Int(&seg_Gpio, i, display_Data[i], 0);
			}
		}
		else if(keypad_Input != 0){
			update_7seg(&seg_Gpio, display_Data, keypad_Input);
		}

		delay(1000);
	}

	return 0;
}

void update_7seg(Seg_TypeDef* seg_Data, int* display_Data, uint16_t keypad_Input){

	int temp = 0;

	for(int i = 0; i < 16; i++){
		if((keypad_Input >> i) & 0x01){
			temp = temp + i;
		}
	}

	if(temp >= 10 && display_Data[6] == -1){
		for(int i = 7; i > 1; i--){
			display_Data[i] = display_Data [i -2];
		}

		display_Data[0] = temp % 10;
		temp = temp/10;
		display_Data[1] = temp;
	}
	else if( display_Data[7] == -1 && temp < 10){
		for(int i = 7; i > 0; i--){
			display_Data[i] = display_Data[i-1];
		}
		display_Data[0] = temp;
	}

	for(int i = 0; i < 8; i++){
		send_7seg_Int(seg_Data, i, display_Data[i], 0);
	}

	return;
}
