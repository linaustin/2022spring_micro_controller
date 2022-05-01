#include "button_function.h"

int button_Init(GPIO_TypeDef* gpio, int pin){

	if(gpio == GPIOC){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}

	else{
		return -1;
	}

	gpio->MODER &= ~(0b11 << (2*pin));
	gpio->MODER |= (0b00 << (2*pin));

	gpio->PUPDR &= ~(0b11 << (2*pin));
	gpio->PUPDR |= (0b01 << (2*pin));

	return 0;
}

int button_Read(GPIO_TypeDef* gpio, int pin){

	int count = 0;
	int button_State = 1;
	int read_State = 0;

	for(int i = 0; i < 100; i++){
		read_State = (gpio->IDR >> pin) & 0b1;

		if(read_State == 0){
			count++;
		}

		delay_Loop(1);
	}

	if(count > 70){
		button_State = 0;
	}


	return button_State;
}

int button_Read_RisingEdge(GPIO_TypeDef *gpio, int pin, int pressed_State){
	int release_State = 1 - pressed_State;
	int isRising = 0;

	int button_State = release_State;
	int last_Button_State = pressed_State;

	for(int i = 0; i < 10; i++){
		button_State = button_Read(gpio, pin);

		if((button_State == pressed_State) && (last_Button_State == release_State)){
			isRising = 1;
		}

		last_Button_State = button_State;
	}

	return isRising;
}

