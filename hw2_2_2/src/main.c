#include "gpio_function.h"
#include "stm32l476xx.h"
#include "button_function.h"

#define LED_gpio GPIOA

#define LED1_pin 5
#define LED2_pin 6
#define LED3_pin 7
#define LED4_pin 8

#define BUTTON_gpio GPIOC

#define BUTTON_pin 13

int button_State = 1;
int last_Button_State = 1;

void read_Led_State(GPIO_TypeDef* gpio, int pin, int *state);

int main(){

	int direction = 0;
	int led_State = 0;
	int led_Data = 0b0001;
	int led_Pin[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};

	for(int i = 0; i < 4; i++){

		if(init_Gpio(LED_gpio, led_Pin[i]) != 0){

			return -1;
		}
	}

	if(init_Button(BUTTON_gpio, BUTTON_pin) != 0){

		return -1;
	}

	for(int i = 0 ; i < 4; i++ ){

			set_Gpio(LED_gpio, led_Pin[i]);
	}

	while(1){

		if(led_State != 0){

			for(int i = 0 ; i < 4; i++ ){

				if((led_Data >> i) & 0b1){

					reset_Gpio(LED_gpio, led_Pin[i]);
				}
				else{

					set_Gpio(LED_gpio, led_Pin[i]);
				}
			}

			if(led_Data == 0b1000 || led_Data == 0b0001){

				direction = 1 - direction;
			}

			if (direction == 1){

				led_Data = (led_Data << (1));
			}
			else{

				led_Data = (led_Data >> (1));
			}
		}

		read_Led_State(BUTTON_gpio, BUTTON_pin, &led_State);
	}

	return 0;
}

void read_Led_State(GPIO_TypeDef* gpio, int pin, int *state){

	for(int i = 0; i < 10; i++){

		button_State = read_Button(gpio, pin);

		if(button_State == 0 && last_Button_State == 1){

			*state = 1 - *state;
		}

		last_Button_State = button_State;
	}

	return;
}
