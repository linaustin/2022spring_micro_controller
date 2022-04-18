#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include "stm32l476xx.h"
#include "keypad_function.h"
#include "7seg_function.h"
#include "gpio_function.h"


uint8_t command_Pointer;
uint8_t error_Flag;
uint8_t clean_Flag;
uint8_t display_Push_Flag;
uint8_t display_Operator;

int input_Buffer[3];
int display_Data[8];
int command_Buffer[50];

int calculator_Init(Keypad_TypeDef* keypad_Data, Seg_TypeDef* seg_Data);
int calculator_Interrupt(Keypad_TypeDef* keypad_Data);

void calculator_Clean(Seg_TypeDef* seg_Data);
void calculator_Run(Keypad_TypeDef* keypad_Data, Seg_TypeDef* seg_Data);
void calculator_Display(Seg_TypeDef* seg_Data);

int calculator_Cal(int* command);

#endif
