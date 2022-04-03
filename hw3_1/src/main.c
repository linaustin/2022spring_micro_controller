#include "stm32l476xx.h"
#include "gpio_function.h"
#include "7seg_function.h"

#define seg_gpio GPIOB
#define seg_DIN 3
#define seg_CS	4
#define seg_CLK 5

int main(){

	int data[7] = {0, 8, 1, 1, 0, 7, 4};

	if(init_7seg(seg_gpio, seg_DIN, seg_CS, seg_CLK) != 0){
		return -1;
	}

	send_7seg_Char(7,'-');

	for(int i = 0; i < 7; i++){
		send_7seg_Int((6-i), data[i]);
	}

	return 0;
}


