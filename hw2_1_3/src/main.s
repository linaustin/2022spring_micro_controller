.syntax unified
.cpu cortex-m4
.thumb

.text
	.global main
    .include "../src/pin.s"


 // R0 address of target register
 // R1 value of register

 //led pins pb3 pb4 pb5 pb10
 //switch pins pc0 pc1 pc2 pc3

gpio_init:

	//enable GPIOB and GPIOC
	LDR R0, =RCC_AHB2ENR
	LDR R1, [R0]
	AND R1, #0xFFFFFFF9
	ORR R1, #0x00000006
	STR R1, [R0]

	//setting GPIOB_MODER to output mode
	LDR R0, =GPIOB_MODER
	LDR R1, [R0]
	LDR R2, =0xFFCFF03F
	AND R1, R2
	LDR R2, =0x00100540
	ORR R1, R2
	STR R1, [R0]

	//setting GPIOB_OTYPER to push pull
	LDR R0, =GPIOB_OTYPER
	LDR R1, [R0]
	AND R1, #0xFFFFFBC7
	ORR R1, #0x00000000
	STR R1, [R0]

	//setting GPIOB_ODR to zero
	LDR R0, =GPIOB_ODR
	LDR R1, [R0]
	LDR R2, =0xFFFF0000
	AND R1, R2
	STR R1, [R0]

	//setting GPIOC_MODER¡@to input mode
	LDR R0, =GPIOC_MODER
	LDR R1, [R0]
	AND R1, #0xFFFFFF00
	ORR R1, #0x00000000
	STR R1, [R0]

	//setting GPIOC_OTYPER
	LDR R0, =GPIOC_OTYPER
	LDR R1, [R0]
	AND R1, #0xFFFFFFF0
	ORR R1, #0x00000000
	STR R1, [R0]

	//setting GPIOC_PUPDR
	LDR R0, =GPIOC_PUPDR
	LDR R1, [R0]
	AND R1, #0xFFFFFF00
	ORR R1, #0x00000055
	STR R1, [R0]

	//back to main branch
	MOV PC, LR

led_on:

	//setting GPIOB_ODR
	LDR R0, =GPIOB_ODR
	LDR R1, [R0]
	AND R1, #0xFFFFFBC7
	LDR R2, =0x0000FBC7
	ORR R1, R2
	STR R1, [R0]

	//back to input_check branch
	MOV PC, LR

led_off:

	//setting GPIOB_ODR
	LDR R0, =GPIOB_ODR
	LDR R1, [R0]
	LDR R2, =0x0000FFFF
	ORR R1, R2
	STR R1, [R0]

	//back to input_check branch
	MOV PC, LR

input_check:

	//read GPIOC_IDR to R1
	LDR R0, =GPIOC_IDR
	LDR R1, [R0]

	//setting secret code
	MOV R5, #0x03

	//compare idr and secret code
	CMP R1, R5

	IT EQ
	BLEQ led_on

	IT NE
	BLNE led_off

	//loop
	B input_check

main:
	BL gpio_init
	B input_check


