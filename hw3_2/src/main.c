#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"

#define seg_gpio GPIOB
#define seg_DIN 3
#define seg_CS	4
#define seg_CLK 5

#define button_gpio GPIOC
#define button_Pin 13

int fib_Calculation(int N, int* fib);
uint8_t read_Button_State();
void display_Number(int number);

int main(){
	int fib[2] = {1,1};
	int N = 0;
	uint8_t button_Pressed = 0;

	if(init_7seg(seg_gpio, seg_DIN, seg_CS, seg_CLK) != 0){
		return -1;
	}

	if(init_Button(button_gpio, button_Pin) != 0){
		return -1;
	}

	display_Number(0);

	while(1){
		button_Pressed = read_Button_State();

		if(button_Pressed){
			N++;
			display_Number(fib_Calculation(N, fib));
		}
	}

	return 0;
}

int fib_Calculation(int N, int* fib){
	int temp;

	if(N < 3){
		return fib[1];
	}
	else{
		temp = fib[0]+ fib[1];

		if(temp >= 100000000){
				return -1;
			}

		fib[0] = fib[1];
		fib[1] = temp;
	}

	return temp;
}

uint8_t read_Button_State(){
	int read;
	int last_read = 0;
	uint8_t button_Pressed = 0;

	for(int i = 0; i < 10; i++){
		read = read_Button(button_gpio, button_Pin);

		if(read == 0 && last_read == 1){
			button_Pressed = 1;
		}
		else{
			last_read = read;
		}
	}

	return button_Pressed;
}

void display_Number(int number){
	int digit = 0;

	if(number < 0){
		number = number * -1;

		while(number != 0){
			send_7seg_Int(digit, number % 10);
			number = number / 10;
			digit++;
		}

		send_7seg_Char(digit, '-');
		digit++;

		while(digit < 8){
			send_7seg_Char(digit, ' ');
			digit++;
		}
	}
	else{
		if(number == 0){
			send_7seg_Int(digit, 0);
			digit++;
		}

		while(number != 0){
			send_7seg_Int(digit, number % 10);
			number = number / 10;
			digit++;
		}

		while(digit < 8){
			send_7seg_Char(digit, ' ');
			digit++;
		}
	}

	return;
}
