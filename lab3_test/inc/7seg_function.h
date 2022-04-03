#ifndef __7SEG_FUNCTION_H__
#define __7SEG_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"

typedef struct{
	GPIO_TypeDef* gpio;
	int DIN;
	int CS;
	int CLK;
}seg_Pin_Data;

seg_Pin_Data seg_Data;
seg_Pin_Data* seg_Gpio;

int init_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLK);

void send_7seg_Msg(uint8_t address, uint8_t data);


// Max7219 Serial_Data_Address
#define Seg_No_Op_Address  			0b0000
#define Seg_Digit0_Address 			0b0001
#define Seg_Digit1_Address 			0b0010
#define Seg_Digit2_Address 			0b0011
#define Seg_Digit3_Address 			0b0100
#define Seg_Digit4_Address 			0b0101
#define Seg_Digit5_Address 			0b0110
#define Seg_Digit6_Address 			0b0111
#define Seg_Digit7_Address 			0b1000
#define Seg_Decode_Mode_Address 	0b1001
#define Seg_Intensity_Address 		0b1010
#define Seg_Scan_Limit_Address 		0b1011
#define Seg_Shutdown_Address 		0b1100
#define Seg_Display_Test_Address 	0b1111

//Max7219 Decode_Mode_Data
#define Seg_All_Decode		0xFF
#define Seg_All_No_Decode 	0x00

//Max7219 Shutdown_Data
#define Seg_Shutdown_Data	0x00
#define Seg_Wakeup_Data		0x01

//Max7219 Intensity_Data
#define Seg_Intensity_0 	0x00
#define Seg_Intensity_1 	0x01
#define Seg_Intensity_2 	0x02
#define Seg_Intensity_3 	0x03
#define Seg_Intensity_4 	0x04
#define Seg_Intensity_5 	0x05
#define Seg_Intensity_6 	0x06
#define Seg_Intensity_7 	0x07
#define Seg_Intensity_8	 	0x08
#define Seg_Intensity_9 	0x09
#define Seg_Intenstiy_10 	0x0A
#define Seg_Intensity_11	0x0B
#define Seg_Intensity_12	0x0C
#define Seg_Intensity_13	0x0D
#define Seg_Intensity_14	0x0E
#define Seg_Intensity_15	0x0F

//Max7219 Decode_Mode_Data
#define Seg_Decode_Data_0		0x00
#define Seg_Decode_Data_1		0x01
#define Seg_Decode_Data_2		0x02
#define Seg_Decode_Data_3		0x03
#define Seg_Decode_Data_4		0x04
#define Seg_Decode_Data_5		0x05
#define Seg_Decode_Data_6		0x06
#define Seg_Decode_Data_7		0x07
#define Seg_Decode_Data_8		0x08
#define Seg_Decode_Data_9		0x09
#define Seg_Decode_Data_Dash	0x0A
#define Seg_Decode_Data_E		0x0B
#define Seg_Decode_Data_H		0x0C
#define Seg_Decode_Data_L		0x0D
#define Seg_Decode_Data_P		0x0E
#define Seg_Decode_Data_Blank	0x0F

//Max7219 No_Decode_Mode_Data
#define Seg_No_Decode_Data_0 		0x7E
#define Seg_No_Decode_Data_1    	0x30
#define Seg_No_Decode_Data_2		0x6D
#define Seg_No_Decode_Data_3 		0x79
#define Seg_No_Decode_Data_4		0x33
#define Seg_No_Decode_Data_5 		0x5B
#define Seg_No_Decode_Data_6		0x5F
#define Seg_No_Decode_Data_7 		0x70
#define Seg_No_Decode_Data_8		0x7F
#define	Seg_No_Decode_Data_9		0x7B
#define Seg_No_Decode_Data_Dash		0x01
#define Seg_No_Decode_Data_E		0x4F
#define Seg_No_Decode_Data_H		0x37
#define Seg_No_Decode_Data_L		0x0E
#define Seg_No_Decode_Data_P		0x67
#define Seg_No_Decode_Data_Blank	0x00

#endif
