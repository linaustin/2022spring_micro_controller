#ifndef __SYSTEM_FUNCTION_H__
#define __SYSTEM_FUNCTION_H__

#include "stm32l476xx.h"

typedef struct sysclk_Data_Struct{
	// sysclk = source * plln/2* (pllr + 1) * (pllm + 1)
	uint8_t plln;	//8 <= plln <= 86
	uint8_t pllr;	//0 <= pllr <= 3
	uint8_t pllm;	//0 <= pllm <= 7
}Sysclk_Data;

void FPU_Init();
void delay_Loop(int msec);
void system_Clock_Config(Sysclk_Data *sysclk_Data);

Sysclk_Data sysclk_1Mhz;
Sysclk_Data sysclk_6Mhz;
Sysclk_Data sysclk_10Mhz;
Sysclk_Data sysclk_16Mhz;
Sysclk_Data sysclk_20Mhz;
Sysclk_Data sysclk_40Mhz;

#endif
