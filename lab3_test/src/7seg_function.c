#include"7seg_function.h"

int init_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLK){

	if(init_Gpio(gpio, DIN) != 0 || init_Gpio(gpio, CS) != 0 || init_Gpio(gpio, CLK) != 0){
		return -1;
	}
	else{
		seg_Gpio->gpio = gpio;
		seg_Gpio->DIN = DIN;
		seg_Gpio->CLK = CLK;
		seg_Gpio->CS = CS;
	}

	reset_Gpio(seg_Gpio->gpio, seg_Gpio->DIN);
	reset_Gpio(seg_Gpio->gpio, seg_Gpio->CLK);
	reset_Gpio(seg_Gpio->gpio, seg_Gpio->CS);

	all_Decode_Mode();
	send_7seg_Msg(Seg_Intensity_Address, Seg_Intensity_15);
	send_7seg_Msg(Seg_Scan_Limit_Address, 0x07);
	send_7seg_Msg(Seg_Shutdown_Address, 0x01);
	send_7seg_Msg(Seg_Display_Test_Address, 0x00);

	return 0;
}

void send_7seg_Msg(uint8_t address, uint8_t data){

	uint16_t msg = 0x00;
	msg |= (address << (8));
	msg |= data;

	for(int i = 0; i < 17; i++){
		switch(i){
			case 16:
				set_Gpio(seg_Gpio->gpio , seg_Gpio->CS);
				set_Gpio(seg_Gpio->gpio, seg_Gpio->CLK);
				break;

			default:
				reset_Gpio(seg_Gpio->gpio, seg_Gpio->CLK);

				if((msg >> (i)) & 0x01){
					set_Gpio(seg_Gpio->gpio, seg_Gpio->DIN);
				}
				else{
					reset_Gpio(seg_Gpio->gpio, seg_Gpio->DIN);
				}

				reset_Gpio(seg_Gpio->gpio, seg_Gpio->CS);

				set_Gpio(seg_Gpio->gpio, seg_Gpio->CLK);
				break;
		}
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
		send_7seg_Msg(address, msg);
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
		send_7seg_Msg(address, msg);
	}

	return ;
}
