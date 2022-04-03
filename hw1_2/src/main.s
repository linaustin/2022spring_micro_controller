	.syntax unified
	.cpu cortex-m4
	.thumb

	.data

	.text
		.global main

main:

	mov r1, #0x05
	mov r2, #0x09

	mul r0, r1, r2


