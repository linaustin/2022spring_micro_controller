#include "stm32l476xx.h"
#include "gpio_function.h"
#include "button_function.h"
#include "7seg_function.h"
#include <stdlib.h>


#define seg_Gpio GPIOB
#define seg_DIN 3
#define seg_CS	4
#define seg_CLK 5

#define button_Gpio GPIOC
#define button_Pin 13

int fib_Calculation(int N, int* fib);
void read_Button_State(uint8_t *button_Pressed);
void display_Number(int number);

int main(){
	int fib[2] = {1,1};
	int N = 0;
	uint8_t button_Pressed = 0;

	if(init_7seg(seg_Gpio, seg_DIN, seg_CS, seg_CLK) != 0){
		return -1;
	}

	if(init_Button(button_Gpio, button_Pin) != 0){
		return -1;
	}

	while(1){
		read_Button_State(&button_Pressed);

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

void read_Button_State(uint8_t *button_Pressed){
	int read;
	int last_read = 1;

	for(int i = 0; i < 10; i++){
		read = read_Button(button_Gpio, button_Pin);

		if(read == 0 && last_read == 1){
			*button_Pressed = 1;
			return;
		}
		else{
			last_read = read;
		}
	}

	return;
}

void display_Number(int number){
	if(number == -1){
		send_7seg_Char(1, '-');
		send_7seg_Int(0, 1);
		return;
	}

	int count = 0;
	int temp = number;

	while(number > 0){
		temp = temp/10;
		count++;
	}

	if(count > 8){
		send_7seg_Char(1, '-');
		send_7seg_Int(0, 1);
		return;
	}

	int *arr;

	arr = (int*)calloc(count, sizeof(int));

	for(int i = 0; i < count; i++){
		arr[i] = number%10;
		number = number/10;
	}

	for(int i = 0; i < 8; i++){
		if(i < count){
			send_7seg_Int(i, arr[i]);
		}
		else{
			send_7seg_Char(i, ' ');
		}
	}

	return;
}
