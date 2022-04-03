#include "gpio_function.h"
#include "stm32l476xx.h"
#include "button_function.h"

#define LED_gpio GPIOB

#define LED1_pin 3
#define LED2_pin 4
#define LED3_pin 5
#define LED4_pin 6

#define BUTTON_gpio GPIOC

#define BUTTON_pin 13

void read_Led_Mode(GPIO_TypeDef* gpio, int pin, int* mode);

int button_State = 1;
int last_Button_State = 1;

int main(){

	int direction = 0;
	int led_Mode = 0;
	int led_Original = 0b00000111;
	int led_Shield[3]= {0b00000111, 0b00000110, 0b00000100};
	int led_Data = 0;
	int led_Pin[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};

	for(int i = 0; i < 4; i++){

		if(init_Gpio(LED_gpio, led_Pin[i]) != 0){

			return -1;
		}
	}

	if(init_Button(BUTTON_gpio, BUTTON_pin) != 0){

		return -1;
	}

	while(1){

		led_Data = led_Original;
		led_Data &= led_Shield[led_Mode];

		for(int i = 0 ; i < 4; i++ ){

			if((led_Data >> (i+2)) & 0b1){

				reset_Gpio(LED_gpio, led_Pin[i]);
			}
			else{

				set_Gpio(LED_gpio, led_Pin[i]);
			}
		}

		if(direction == 0 && !(led_Data & 0b00001000) && (led_Data & 0b00000100)){

			direction = 1;
		}

		else if (direction == 1 && !(led_Data & 0b00010000) && (led_Data & 0b00100000)){

			direction = 0;
		}

		if(direction == 1){

			led_Original = led_Original << (1);

			for(int i = 0; i < 3; i++){

				led_Shield[i] = led_Shield[i] << (1);
			}
		}
		else{

			led_Original = led_Original >> (1);

			for(int i = 0; i < 3; i++){

				led_Shield[i] = led_Shield[i] >> (1);
			}
		}

		read_Led_Mode(BUTTON_gpio, BUTTON_pin, &led_Mode);
	}

	return 0;
}

void read_Led_Mode(GPIO_TypeDef* gpio, int pin, int* mode){

	for(int i = 0; i < 10; i++){

		button_State = read_Button(gpio, pin);

		if(button_State == 0 && last_Button_State == 1){

			*mode = *mode + 1;

			if(*mode > 2){

				*mode = 0;
			}
		}

		last_Button_State = button_State;
	}

	return;
}


