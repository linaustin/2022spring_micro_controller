#include"7seg_function.h"

uint8_t  decode_State = 0;

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

	seg_Gpio.gpio = gpio;
	seg_Gpio.DIN = DIN;
	seg_Gpio.CLK = CLK;
	seg_Gpio.CS = CS;


	all_Decode_Mode();
	send_7seg_Msg(Seg_Intensity_Address, Seg_Intensity_10);
	send_7seg_Msg(Seg_Scan_Limit_Address, 0x07);
	send_7seg_Msg(Seg_Shutdown_Address, Seg_Wakeup_Data);
	send_7seg_Msg(Seg_Display_Test_Address, 0x00);

	for(int i = 0; i < 8; i++){
		send_7seg_Int(i,-1);
	}

	return 0;
}

void send_7seg_Msg(uint8_t address, uint8_t data){
	uint16_t msg = 0;
	msg |= address;
	msg = msg << (8);
	msg |= data;

	for(int i = 0; i < 17; i++){

		reset_Gpio(seg_Gpio.gpio, seg_Gpio.CLK);

		if(i < 16){
			if((msg >> (15-i)) & 0x01){
				set_Gpio(seg_Gpio.gpio, seg_Gpio.DIN);
			}
			else{
				reset_Gpio(seg_Gpio.gpio, seg_Gpio.DIN);
			}

			reset_Gpio(seg_Gpio.gpio, seg_Gpio.CS);
		}
		else{
			reset_Gpio(seg_Gpio.gpio, seg_Gpio.DIN);
			set_Gpio(seg_Gpio.gpio, seg_Gpio.CS);
		}

		set_Gpio(seg_Gpio.gpio, seg_Gpio.CLK);
	}

	return;
}

void all_Decode_Mode(){
	decode_State = 0;

	send_7seg_Msg(Seg_Decode_Mode_Address, Seg_All_Decode);

	return;
}

void all_No_Decode_Mode(){
	decode_State = 1;

	send_7seg_Msg(Seg_Decode_Mode_Address, Seg_All_No_Decode);

	return;
}

void send_7seg_Int(int digit, int data){
	uint8_t address = (uint8_t)(digit+1);
	uint8_t msg = 0;

	if(decode_State == 0){
		switch(data){
			case 0:
				msg = Seg_Decode_Data_0;
				break;

			case 1:
				msg = Seg_Decode_Data_1;
				break;

			case 2:
				msg = Seg_Decode_Data_2;
				break;

			case 3:
				msg = Seg_Decode_Data_3;
				break;

			case 4:
				msg = Seg_Decode_Data_4;
				break;

			case 5:
				msg = Seg_Decode_Data_5;
				break;

			case 6:
				msg = Seg_Decode_Data_6;
				break;

			case 7:
				msg = Seg_Decode_Data_7;
				break;

			case 8:
				msg = Seg_Decode_Data_8;
				break;

			case 9:
				msg = Seg_Decode_Data_9;
				break;

			default:
				msg = Seg_Decode_Data_Blank;
				break;
		}
	}
	else{
		switch(data){
			case 0:
				msg = Seg_No_Decode_Data_0;
				break;

			case 1:
				msg = Seg_No_Decode_Data_1;
				break;

			case 2:
				msg = Seg_No_Decode_Data_2;
				break;

			case 3:
				msg = Seg_No_Decode_Data_3;
				break;

			case 4:
				msg = Seg_No_Decode_Data_4;
				break;

			case 5:
				msg = Seg_No_Decode_Data_5;
				break;

			case 6:
				msg = Seg_No_Decode_Data_6;
				break;

			case 7:
				msg = Seg_No_Decode_Data_7;
				break;

			case 8:
				msg = Seg_No_Decode_Data_8;
				break;

			case 9:
				msg = Seg_No_Decode_Data_9;
				break;

			default:
				msg = Seg_No_Decode_Data_Blank;
				break;
		}
	}

	send_7seg_Msg(address, msg);

	return;
}

void send_7seg_Char(int digit, char data){
	uint8_t address = (uint8_t)(digit+1);
	uint8_t msg = 0x00;

	if(decode_State == 0){
		switch(data){
			case '-':
				msg = Seg_Decode_Data_Dash;
				break;

			case 'e':
				msg = Seg_Decode_Data_E;
				break;

			case 'h':
				msg = Seg_Decode_Data_H;
				break;

			case 'l':
				msg = Seg_Decode_Data_L;
				break;

			case 'p':
				msg = Seg_Decode_Data_P;
				break;

			case ' ':
			default:
				msg = Seg_Decode_Data_Blank;
				break;
		}
	}
	else{
		switch(data){
			case '-':
				msg = Seg_No_Decode_Data_Dash;
				break;

			case 'e':
				msg = Seg_No_Decode_Data_E;
				break;

			case 'h':
				msg = Seg_No_Decode_Data_H;
				break;

			case 'l':
				msg = Seg_No_Decode_Data_L;
				break;

			case 'p':
				msg = Seg_No_Decode_Data_P;
				break;

			case ' ':
			default:
				msg = Seg_No_Decode_Data_Blank;
				break;
		}
	}

	send_7seg_Msg(address, msg);

	return;
}
