#include"keypad_function.h"

const int  keypad_Map[4][4] = {
		{1,2,3,10},
		{4,5,6,11},
		{7,8,9,12},
		{15,0,14,13}
};

int init_Keypad(Keypad_TypeDef* keypad_Data){
	for(int i = 0; i < 4; i++){
		if(keypad_Data->in_Gpio[i] == GPIOA){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		}
		else if(keypad_Data->in_Gpio[i] == GPIOB){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		}
		else if(keypad_Data->in_Gpio[i] == GPIOC){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		}
		else{
			return -1;
		}
	}

	for(int i = 0; i < 4; i++){
		if(keypad_Data->out_Gpio[i] == GPIOA){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		}
		else if(keypad_Data->out_Gpio[i] == GPIOB){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		}
		else if(keypad_Data->out_Gpio[i] == GPIOC){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		}
		else{
			return -1;
		}
	}

	for(int i = 0; i < 4; i++){
		keypad_Data->out_Gpio[i]->MODER &= ~(0b11 << (2 * keypad_Data->out_Pin[i]));
		keypad_Data->out_Gpio[i]->MODER |= (0b01 << (2 * keypad_Data->out_Pin[i]));

		keypad_Data->out_Gpio[i]->OSPEEDR &= ~(0b11 << (2 * keypad_Data->out_Pin[i]));
		keypad_Data->out_Gpio[i]->OSPEEDR |= (0b11 << (2 * keypad_Data->out_Pin[i]));

		keypad_Data->out_Gpio[i]->OTYPER &= ~(0b01 << (keypad_Data->out_Pin[i]));
		keypad_Data->out_Gpio[i]->OTYPER |= (0b01 << (keypad_Data->out_Pin[i]));

		set_Gpio(keypad_Data->out_Gpio[i], keypad_Data->out_Pin[i]);
	}

	for(int i = 0; i < 4; i++){
		keypad_Data->in_Gpio[i]->MODER &= ~(0b11 << (2 * keypad_Data->in_Pin[i]));
		keypad_Data->in_Gpio[i]->MODER |= (0b00 << (2 * keypad_Data->in_Pin[i]));

		keypad_Data->in_Gpio[i]->PUPDR &= ~(0b11 << (2 * keypad_Data->in_Pin[i]));
		keypad_Data->in_Gpio[i]->PUPDR |= (0b10 << (2 * keypad_Data->in_Pin[i]));

		reset_Gpio(keypad_Data->in_Gpio[i], keypad_Data->in_Pin[i]);
	}

	return 0;
}

uint16_t read_Keypad(Keypad_TypeDef* keypad_Data){
	uint16_t input = 0;
	int cnt[4][4];
	int scan_Cycle = 400;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			cnt[i][j] = 0;
		}
	}

	for(int count = 0; count < scan_Cycle; count++){
		for(int j = 0; j < 4; j++){
			keypad_Data->out_Gpio[j]->OTYPER &= ~(0b01 << (keypad_Data->out_Pin[j]));

			for(int i = 0; i < 4; i++){
					cnt[i][j] += read_Gpio(keypad_Data->in_Gpio[i], keypad_Data->in_Pin[i]);
				}

			keypad_Data->out_Gpio[j]->OTYPER |= (0b01 << (keypad_Data->out_Pin[j]));
		}
	}

	for(int i = 0; i < 4; i ++){
		for(int j = 0; j < 4; j++){
			if(cnt[i][j] > 150){
				input |= (0b01 << (keypad_Map[i][j]));
			}
		}
	}

	return input;
}
