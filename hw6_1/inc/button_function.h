#ifndef __BUTTON_FUNCTION_H__
#define __BUTTON_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"
#include "system_function.h"

int button_Init(GPIO_TypeDef *gpio, int pin);
int button_Read(GPIO_TypeDef *gpio, int pin);
int button_Read_RisingEdge(GPIO_TypeDef *gpio, int pin, int pressed_State);

#endif
