#include <gpio_function.h>

int init_Gpio(GPIO_TypeDef* gpio, int pin){

	if(gpio == GPIOA){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	}
	else if(gpio == GPIOB){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}
	else if(gpio == GPIOC){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}
	else{
		return -1;
	}

	gpio->MODER &= ~(0b11 << (2 * pin));
	gpio->MODER |= (0b01 << (2 * pin));

	return 0;
}

int read_Gpio(GPIO_TypeDef* gpio, int pin){

	return (gpio->IDR >> (pin)) & 0b01;
}


void set_Gpio(GPIO_TypeDef* gpio, int pin){

	gpio->BSRR |= (0b01 << pin);

	return;
}

void reset_Gpio(GPIO_TypeDef* gpio, int pin){

	gpio->BRR |= (0b01 << pin);

	return;
}

void toggle_Gpio(GPIO_TypeDef* gpio, int pin){

	if(read_Gpio(gpio,pin) == 0){
		set_Gpio(gpio, pin);
	}
	else{
		reset_Gpio(gpio, pin);
	}

	return;
}


void delay(int msec){

	int loop_cnt = 500*msec;

	while(loop_cnt){
		loop_cnt--;
	}

	return;
}
