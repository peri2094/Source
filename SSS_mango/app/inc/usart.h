#ifndef USART_H_
#define USART_H_

#include "hw_config.h"

#define GPIO_USART1                     GPIOA
#define GPIO_USART1_TX                  GPIO_Pin_9
#define GPIO_USART1_RX                  GPIO_Pin_10

#define GPIO_USART3                     GPIOB
#define GPIO_USART3_TX                  GPIO_Pin_10
#define GPIO_USART3_RX                  GPIO_Pin_11

int fputc(int ch, FILE *f);
int fgetc(FILE *f); 
void USART1_Configuration(void);
void USART3_Configuration(void);

void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t *s);

uint16_t USART_RecieveData(USART_TypeDef* USARTx);
uint8_t USART_GetChar(USART_TypeDef* usart_p);

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__*/

#endif