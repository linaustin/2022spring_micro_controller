#include"7seg_function.h"

uint8_t  decode_State = 0;

int init_7seg(Seg_TypeDef* seg_Data){
	if(seg_Data->gpio == GPIOA){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		}
		else if(seg_Data->gpio == GPIOB){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		}
		else if(seg_Data->gpio == GPIOC){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		}
		else{
			return -1;
		}

	seg_Data->gpio->MODER &= ~(0b11 << (2*seg_Data->DIN));
	seg_Data->gpio->MODER |= (0b01 << (2*seg_Data->DIN));
	seg_Data->gpio->MODER &= ~(0b11 << (2*seg_Data->CS));
	seg_Data->gpio->MODER |= (0b01 << (2*seg_Data->CS));
	seg_Data->gpio->MODER &= ~(0b11 << (2*seg_Data->CLK));
	seg_Data->gpio->MODER |= (0b01 << (2*seg_Data->CLK));

	all_No_Decode_Mode();
	send_7seg_Msg(seg_Data, Seg_Intensity_Address, Seg_Intensity_15);
	send_7seg_Msg(seg_Data, Seg_Scan_Limit_Address, 0x07);
	send_7seg_Msg(seg_Data, Seg_Shutdown_Address, Seg_Wakeup_Data);
	send_7seg_Msg(seg_Data, Seg_Display_Test_Address, 0x00);

	for(int i = 0; i < 8; i++){
		send_7seg_Int(seg_Data, i, -1, 0);
	}

	return 0;
}

void send_7seg_Msg(Seg_TypeDef* seg_Data, uint8_t address, uint8_t data){
	uint16_t msg = 0;
	msg |= address;
	msg = msg << (8);
	msg |= data;

	for(int i = 0; i < 17; i++){

		gpio_Reset(seg_Data->gpio, seg_Data->CLK);

		if(i < 16){
			if((msg >> (15-i)) & 0x01){
				gpio_Set(seg_Data->gpio, seg_Data->DIN);
			}
			else{
				gpio_Reset(seg_Data->gpio, seg_Data->DIN);
			}

			gpio_Reset(seg_Data->gpio, seg_Data->CS);
		}
		else{
			gpio_Reset(seg_Data->gpio, seg_Data->DIN);
			gpio_Set(seg_Data->gpio, seg_Data->CS);
		}

		gpio_Set(seg_Data->gpio, seg_Data->CLK);
	}

	return;
}

void all_Decode_Mode(Seg_TypeDef* seg_Data){
	decode_State = 0;

	send_7seg_Msg(seg_Data, Seg_Decode_Mode_Address, Seg_All_Decode);

	return;
}

void all_No_Decode_Mode(Seg_TypeDef* seg_Data){
	decode_State = 1;

	send_7seg_Msg(seg_Data, Seg_Decode_Mode_Address, Seg_All_No_Decode);

	return;
}

void send_7seg_Int(Seg_TypeDef* seg_Data, int digit, int data, uint8_t point){
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

			case 10:
				msg = Seg_No_Decode_Data_P;
				break;

			case 11:
				msg = Seg_No_Decode_Data_S;
				break;

			case 12:
				msg = Seg_No_Decode_Data_H;
				break;

			case 13:
				msg = Seg_No_Decode_Data_d;
				break;

			default:
				msg = Seg_No_Decode_Data_Blank;
				break;
		}
	}

	if(point){
		msg |= 0x80;
	}

	send_7seg_Msg(seg_Data, address, msg);

	return;
}

void send_7seg_Char(Seg_TypeDef* seg_Data, int digit, char data){
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

	send_7seg_Msg(seg_Data, address, msg);

	return;
}

void display_Number_Float2(Seg_TypeDef* seg_Data, int number_float2){
	int temp;
	uint16_t total_Digit = 0;
	uint8_t negative = 0;
	uint8_t decimal = 1; // should be 0 but the timer will display 0.00 as the result set to 1;

	if(number_float2 < 0){
		number_float2 = number_float2 * -1;
		negative = 1;
		total_Digit++;
	}

	if(number_float2 % 100 != 0){
		decimal = 1;
	}

	if(decimal || number_float2 == 0){
		temp = number_float2;

		while(temp > 0){
			temp = temp/10;
			total_Digit++;
		}

		if(total_Digit > 8){
			send_7seg_Error(seg_Data);
			return;
		}

		if(number_float2 < 100 && number_float2 > 0){
			total_Digit++;
		}
		else if(number_float2 > -100 && number_float2 < 0){
			total_Digit++;
		}
		else if( number_float2 == 0){
			total_Digit = 3;
		}

		for(int i = 0; i < 8; i++){
			if(i < total_Digit && i == 2){
				send_7seg_Int(seg_Data, i, number_float2%10, 1);
				number_float2 = number_float2/10;
			}
			else if(i < total_Digit){
				if(negative && i == (total_Digit -1)){
					continue;
				}
				send_7seg_Int(seg_Data, i, number_float2%10, 0);
				number_float2 = number_float2/10;
			}
			else{
				send_7seg_Char(seg_Data, i, ' ');
			}
		}

		if(negative){
			send_7seg_Char(seg_Data, total_Digit-1, '-');
		}
	}
	else{
		number_float2 = number_float2/1000;
		temp = number_float2;

		while(temp > 0){
			temp = temp/10;
			total_Digit++;
		}

		if(total_Digit > 8){
			send_7seg_Error(seg_Data);
			return;
		}

		for(int i = 0; i < 8; i++){
			if(i < total_Digit && i == 0){
				send_7seg_Int(seg_Data, i, number_float2%10, 1);
				number_float2 = number_float2/10;
			}
			else if(i < total_Digit){
				if(negative && i == (total_Digit -1)){
					continue;
				}
				send_7seg_Int(seg_Data, i, number_float2%10, 0);
				number_float2 = number_float2/10;
			}
			else{
				send_7seg_Char(seg_Data, i, ' ');
			}
		}

		if(negative){
			send_7seg_Char(seg_Data, total_Digit-1, '-');
		}
	}

	return;
}

void display_Number_Float3(Seg_TypeDef* seg_Data, int number_float3){
	int temp;
	uint16_t total_Digit = 0;
	uint8_t negative = 0;
	uint8_t decimal = 0;

	if(number_float3 < 0){
		number_float3 = number_float3 * -1;
		negative = 1;
		total_Digit++;
	}

	if(number_float3 % 1000 != 0){
		decimal = 1;
	}

	if(decimal || number_float3 == 0){
		temp = number_float3;

		while(temp > 0){
			temp = temp/10;
			total_Digit++;
		}

		if(total_Digit > 8){
			send_7seg_Error(seg_Data);
			return;
		}

		if(number_float3 < 1000 && number_float3 > 0){
			total_Digit++;
		}
		else if(number_float3 > -1000 && number_float3 < 0){
			total_Digit++;
		}
		else if( number_float3 == 0){
			total_Digit = 4;
		}

		for(int i = 0; i < 8; i++){
			if(i < total_Digit && i == 3){
				send_7seg_Int(seg_Data, i, number_float3%10, 1);
				number_float3 = number_float3/10;
			}
			else if(i < total_Digit){
				if(negative && i == (total_Digit -1)){
					continue;
				}
				send_7seg_Int(seg_Data, i, number_float3%10, 0);
				number_float3 = number_float3/10;
			}
			else{
				send_7seg_Char(seg_Data, i, ' ');
			}
		}

		if(negative){
			send_7seg_Char(seg_Data, total_Digit-1, '-');
		}
	}
	else{
		number_float3 = number_float3/1000;
		temp = number_float3;

		while(temp > 0){
			temp = temp/10;
			total_Digit++;
		}

		if(total_Digit > 8){
			send_7seg_Error(seg_Data);
			return;
		}

		for(int i = 0; i < 8; i++){
			if(i < total_Digit && i == 0){
				send_7seg_Int(seg_Data, i, number_float3%10, 1);
				number_float3 = number_float3/10;
			}
			else if(i < total_Digit){
				if(negative && i == (total_Digit -1)){
					continue;
				}
				send_7seg_Int(seg_Data, i, number_float3%10, 0);
				number_float3 = number_float3/10;
			}
			else{
				send_7seg_Char(seg_Data, i, ' ');
			}
		}

		if(negative){
			send_7seg_Char(seg_Data, total_Digit-1, '-');
		}
	}

	return;
}

void send_7seg_Error(Seg_TypeDef* seg_Data){
	all_No_Decode_Mode(seg_Data);
	send_7seg_Msg(seg_Data, Seg_Digit0_Address, Seg_No_Decode_Data_r);
	send_7seg_Msg(seg_Data, Seg_Digit1_Address, Seg_No_Decode_Data_o);
	send_7seg_Msg(seg_Data, Seg_Digit2_Address, Seg_No_Decode_Data_r);
	send_7seg_Msg(seg_Data, Seg_Digit3_Address, Seg_No_Decode_Data_r);
	send_7seg_Msg(seg_Data, Seg_Digit4_Address, Seg_No_Decode_Data_E);

	for(int i = 5; i < 8; i++){
		send_7seg_Char(seg_Data, i, ' ');
	}

	return;
}

void send_7seg_Plus(Seg_TypeDef* seg_Data){
	all_No_Decode_Mode(seg_Data);
	send_7seg_Msg(seg_Data, Seg_Digit0_Address, Seg_No_Decode_Data_S);
	send_7seg_Msg(seg_Data, Seg_Digit1_Address, Seg_No_Decode_Data_U);
	send_7seg_Msg(seg_Data, Seg_Digit2_Address, Seg_No_Decode_Data_L);
	send_7seg_Msg(seg_Data, Seg_Digit3_Address, Seg_No_Decode_Data_P);

	for(int i = 4; i < 8; i++){
			send_7seg_Char(seg_Data, i, ' ');
	}

	return;
}
void send_7seg_Subtraction(Seg_TypeDef* seg_Data){
	all_No_Decode_Mode(seg_Data);
	send_7seg_Msg(seg_Data, Seg_Digit0_Address, Seg_No_Decode_Data_8);
	send_7seg_Msg(seg_Data, Seg_Digit1_Address, Seg_No_Decode_Data_U);
	send_7seg_Msg(seg_Data, Seg_Digit2_Address, Seg_No_Decode_Data_S);

	for(int i = 3; i < 8; i++){
			send_7seg_Char(seg_Data, i, ' ');
	}

	return;
}
void send_7seg_Multiplie(Seg_TypeDef* seg_Data){
	all_No_Decode_Mode(seg_Data);
	send_7seg_Msg(seg_Data, Seg_Digit0_Address, Seg_No_Decode_Data_L);
	send_7seg_Msg(seg_Data, Seg_Digit1_Address, Seg_No_Decode_Data_U);
	send_7seg_Msg(seg_Data, Seg_Digit2_Address, 0xF6);
	send_7seg_Msg(seg_Data, Seg_Digit3_Address, 0xF6);

	for(int i = 4; i < 8; i++){
			send_7seg_Char(seg_Data, i, ' ');
	}
	return;
}
void send_7seg_Divide(Seg_TypeDef* seg_Data){
	all_No_Decode_Mode(seg_Data);
	send_7seg_Msg(seg_Data, Seg_Digit0_Address, Seg_No_Decode_Data_i);
	send_7seg_Msg(seg_Data, Seg_Digit1_Address, Seg_No_Decode_Data_d);

	for(int i = 2; i < 8; i++){
			send_7seg_Char(seg_Data, i, ' ');
	}

	return;
}

