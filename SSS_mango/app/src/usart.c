#include "usart.h"

/*--------Printf, Getchar semi-hosting part--------*/

int fputc(int ch, FILE *f)
{
  if(ch == '\n')
   {
     USART_SendData(USART3, '\r');
     while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
     USART_SendData(USART3, '\n');
   }
   else 
   {
     USART_SendData(USART3, (uint8_t) ch);
   }
   
   while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
   return ch;
}

int fgetc(FILE *f)
{
  char ch;
  
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  ch = USART_RecieveData(USART1);
  
  return ((int)ch);
}


/*--------USART1 Initialize--------*/

void USART1_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART1_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIO_USART1, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART1_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_USART1, &GPIO_InitStructure);
  
  
  /*
  usart1 setting :
  - baud rate : 115200
  - word length : 8bits
  - one stop bit, no parity
  - Hardware flow control disabled
  - Recieve and Transmit enabled
  */
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Usart1 configure , enable */
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}

void USART3_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART3_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIO_USART3, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART3_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_USART3, &GPIO_InitStructure);
  
  /*
  usart3 setting :
  - baud rate : 115200
  - word length : 8bits
  - one stop bit, no parity
  - Hardware flow control disabled
  - Recieve and Transmit enabled
  */
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Usart3 configure , enable */
  USART_Init(USART3, &USART_InitStructure);
  USART_Cmd(USART3, ENABLE);
}

/*--------USART GPIO Configuration--------*/
/*
void USART_Configuration(void) 
{
  //Enable GPIO_USART CLOCK//
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  //Setting USART GPIO//
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART1_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIO_USART1, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART1_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_USART1, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART3_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIO_USART3, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_USART3_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_USART3, &GPIO_InitStructure);
}
*/
/*--------Usart_getchar Active--------*/

uint8_t USART_GetChar(USART_TypeDef* usart_p)
{
  uint8_t data;
  
  while(USART_GetFlagStatus(usart_p, USART_FLAG_RXNE) == RESET);
  data = USART_RecieveData(usart_p);
  
  USART_SendData(usart_p, data);
  while(USART_GetFlagStatus(usart_p, USART_FLAG_TXE) == RESET);
  
  if(data == '\r') {return (int)('\n');}
  else             {return data;}
}

uint16_t USART_RecieveData(USART_TypeDef* USARTx)
{
  return (uint16_t)(USARTx->DR & ((uint16_t)0x01FF));
}
