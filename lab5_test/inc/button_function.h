#ifndef __BUTTON_FUNCTION_H__
#define __BUTTON_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"
#include "system_function.h"

int init_Button(GPIO_TypeDef* gpio, int pin);
int read_Button(GPIO_TypeDef* gpio, int pin);

#endif
