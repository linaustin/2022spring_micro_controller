#ifndef __TIMER_FUNCTION_H__
#define __TIMER_FUNCTION_H_

#include "stm32l476xx.h"

typedef struct tim_Struct{


}Timer_Data;

typedef struct sysclk_Struct{
	uint8_t pllm;
	uint8_t plln;
	uint8_t pllr;
}Sysclk_Data;

void system_Clock_Config(Sysclk_Data *sysclk_Data);



#endif
