//------------------------------------------------------------------------------------
//	Title 		: 	UART library
//	File		: 	uart.c
//  Author		: 	Lee sungryul
//  Company  	: 	Dept. of Control & Robotics Eng., Kunsan University
//
//	Description	:  	UART library
//	Created  	:  	2010.9.2
//  Revision 	:	2012.9.23 redirection of printf,scanf to UART added		
//
//------------------------------------------------------------------------------------

#ifndef _USART_H_
#define _USART_H_ 

#include "avr/io.h"
#include "avr/interrupt.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

int usart0_getintnum(void);
void usart0_printf(char *fmt,...);
void usart0_init(void);
int usart0_getchar(void);
int usart0_putchar(char data);
void usart0_gets(char *string);
void usart0_puts(char *str);
int uart_getchar(FILE *stream);
int uart_putchar(char data, FILE *stream);

#endif

