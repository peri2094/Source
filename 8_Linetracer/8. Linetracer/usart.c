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

#include <stdio.h>
#include <avr/io.h>
#include "usart.h"

char com0_rx_data,com0_rx_eflg;		
char com1_rx_data,com1_rx_eflg;	

FILE outdev = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE indev = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void usart0_init(void){
	cli();
	UCSR0A=0x20;  // 19200Bps, 8Data, 1 Stop, No Parity
	UCSR0B=0x18;  
	UCSR0C=0x06;
	UBRR0H=0x00;
	//UBRR0L=0x19;// fosc = 8Mhz,19200bps
	//UBRR0L=0x2F;  // fosc = 14.6456Mhz,19200bps
	UBRR0L=0x33;// fosc = 16Mhz,19200bps
	
	stdout = &outdev;
	stdin  = &indev;
	sei();
}

int uart_getchar(FILE *stream)
{
	return usart0_getchar();
}

int uart_putchar(char data, FILE *stream)
{
	return usart0_putchar(data);
}

int usart0_getchar(void)
{
	while((UCSR0A & RX_COMPLETE) == 0);
	return (UDR0);
}

int usart0_putchar(char data)
{
	if(data == '\n'){
		while((UCSR0A & DATA_REGISTER_EMPTY) == 0);
		UDR0 = '\r';
	}
	while((UCSR0A & DATA_REGISTER_EMPTY) == 0);
	UDR0 = data;
	return 0;
}

void usart0_gets(char *string)
{
	char *string2=string;
    char c;
	while((c=usart0_getchar())!='\r')
	{
		if(c=='\b')
		{
			if((int)string2<(int)string)
			{
				usart0_puts(" \b");				
				string--;
			}
		}
		else
		{	
			*string++=c;

		}

	}
	*string='\0';
	usart0_putchar('\n');
}		

void usart0_puts(char *str)
{
	while(*str) usart0_putchar(*str++);
}

/*
SIGNAL(SIG_UART0_RECV)
{
	char status,data;

	status=UCSR0A;
	data=UDR0;
	if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){
		com0_rx_data = data;
		com0_rx_eflg = 1;
	}

}

SIGNAL(SIG_UART1_RECV)
{
char status,data;

	status=UCSR1A;
	data=UDR1;
	if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){
		com2_rx_data = data;
		com2_rx_eflg = 1;
	}
}

*/

int usart0_getintnum(void)
{
	char str[30];
	char *string=str;
	int base=10;
	int minus=0;
	int lastindex;
	int result=0;
	int i;

	usart0_gets(string);

	if(string[0]=='-')
	{	
		minus=1;
		string++;
	}
	if(string[0]=='0' && (string[1]=='x'|| string[1]=='X'))
	{
		base=16;
		string+=2;
	}
	lastindex=strlen(string)-1;
	if(string[lastindex]=='h' || string[lastindex]=='H')
	{
		base=16;
		string[lastindex]=0;
		lastindex--;
	}
	if(base==10)
	{
		result=atoi(string);
		result=minus ? (-1*result):result;
	}
	else
	{
	for(i=0;i<=lastindex;i++)
	{
		if(isalpha(string[i]))
		{
			if(isupper(string[i])) result=(result<<4)+string[i]-'A'+10;
			else result=(result<<4)+string[i]-'a'+10;
		}
		else result=(result<<4)+string[i]-'0';
		}
		result=minus ? (-1*result):result;
		}
	return result;
}
void usart0_printf(char *fmt,...)
{
	va_list ap;
	char string[256];

	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	usart0_puts(string);
	va_end(ap);
}










