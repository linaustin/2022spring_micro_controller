#include "stm32l476xx.h"
#include "gpio_function.h"
#include "7seg_function.h"

#define seg_Gpio GPIOB
#define seg_DIN 3
#define seg_CS	4
#define seg_CLK 5

int main(){



	init_7seg(seg_Gpio, seg_DIN, seg_CS, seg_CLK);

	send_7seg_Msg(Seg_Digit0_Address, Seg_Decode_Data_0);
	send_7seg_Msg(Seg_Digit1_Address, Seg_Decode_Data_0);
	send_7seg_Msg(Seg_Digit2_Address, Seg_Decode_Data_0);


	return 0;
}


