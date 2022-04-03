	.syntax unified
	.cpu cortex-m4
	.thumb

	.data

	.text

		.global main

fab:
		ADD R1, R1, #0x01
		ADD R4, R2, R3

		CMP R0, R1
		IT EQ
		MOVEQ PC, R14

		MOV R3, R2
		MOV R2, R4

		B fab

main:

		MOV R0, #0x06
		MOV R1, #0x00

		MOV R2, #0x01
		MOV R3, #0x00

		BL fab

		MOV R4, R2
		MOV R3, #0x00
		MOV R2, #0x00
