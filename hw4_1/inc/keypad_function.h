#ifndef __KEYPAD_FUNCTION_H__
#define __KEYPAD_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"

struct keypad_Struct{
	GPIO_TypeDef* out_1_gpio;
	int out_1_Pin;

	GPIO_TypeDef* out_2_gpio;
	int out_2_Pin;

	GPIO_TypeDef* out_3_gpio;
	int out_3_Pin;

	GPIO_TypeDef* out_4_gpio;
	int out_4_Pin;

	GPIO_TypeDef* in_1_gpio;
	int in_1_Pin;

	GPIO_TypeDef* in_2_gpio;
	int in_2_Pin;

	GPIO_TypeDef* in_3_gpio;
	int in_3_Pin;

	GPIO_TypeDef* in_4_gpio;
	int in_4_Pin;
};

int init_Keypad(struct keypad_Struct* keypad);
uint16_t read_Keypad(struct keypad_Struct* keypad);

#endif
