#ifndef __KEYPAD_FUNCTION_H__
#define __KEYPAD_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"

typedef struct keypad_Struct{
	GPIO_TypeDef* out_Gpio[4];
	int out_Pin[4];

	GPIO_TypeDef* in_Gpio[4];
	int in_Pin[4];
}Keypad_TypeDef;

const int keypad_Map[4][4];

int init_Keypad(Keypad_TypeDef* keypad_Data);
uint16_t read_Keypad(Keypad_TypeDef* keypad_Data);

#endif

