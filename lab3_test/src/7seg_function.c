#include"7seg_function.h"

seg_Pin_Data* seg_Gpio = &seg_Data;

int init_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLK){
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

	gpio->MODER &= ~(0b11 << (2*DIN));
	gpio->MODER |= (0b01 << (2*DIN));
	gpio->MODER &= ~(0b11 << (2*CS));
	gpio->MODER |= (0b01 << (2*CS));
	gpio->MODER &= ~(0b11 << (2*CLK));
	gpio->MODER |= (0b01 << (2*CLK));

	seg_Gpio->gpio = gpio;
	seg_Gpio->DIN = DIN;
	seg_Gpio->CS = CS;
	seg_Gpio->CLK = CLK;

	send_7seg_Msg(Seg_Display_Test_Address, 0x00);
	send_7seg_Msg(Seg_Decode_Mode_Address, 0xFF);
	send_7seg_Msg(Seg_Scan_Limit_Address, 0x02);
	send_7seg_Msg(Seg_Shutdown_Address, 0x01);

	return 0;
}

void send_7seg_Msg(uint8_t address, uint8_t data){

	uint16_t msg = 0;
	msg |= address & 0xFF;
	msg = msg << (8);
	msg |= data &0xFF;

	for(int i = 0; i < 17; i++){

		reset_Gpio(seg_Gpio->gpio, seg_Gpio->CLK);

		if(i < 16){
			if((msg >> (15-i)) & 0x01){
				set_Gpio(seg_Gpio->gpio, seg_Gpio->DIN);
			}
			else{
				reset_Gpio(seg_Gpio->gpio, seg_Gpio->DIN);
			}
			reset_Gpio(seg_Gpio->gpio, seg_Gpio->CS);
		}
		else{
			reset_Gpio(seg_Gpio->gpio, seg_Gpio->DIN);
			set_Gpio(seg_Gpio->gpio, seg_Gpio->CS);
		}

		set_Gpio(seg_Gpio->gpio, seg_Gpio->CLK);
	}

	return;
}




