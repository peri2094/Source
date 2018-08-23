#include "key.h"
#include "led.h"

/*--------KEY (0 ~ 3)Input Initialization--------*/

void GPIO_KEY0_Configuration(void) 
{
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef KEY0_Set;
  
  KEY0_Set.GPIO_Pin = GPIO_KEY_0;
  KEY0_Set.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
  
  GPIO_Init(GPIO_KEY, &KEY0_Set);
}

void GPIO_KEY1_Configuration(void)
{
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef KEY1_Set;
  
  KEY1_Set.GPIO_Pin = GPIO_KEY_1;
  KEY1_Set.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
  
  GPIO_Init(GPIO_KEY, &KEY1_Set);
}

void GPIO_SWITCH_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //Switch X, Y GPIO Input Configuration//
  GPIO_InitStructure.GPIO_Pin = GPIO_SWITCH_X;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIO_SWITCHX, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_SWITCH_Y;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIO_SWITCHY, &GPIO_InitStructure);
}


void KEY_Configuration(void) 
{
  GPIO_KEY0_Configuration();
  GPIO_KEY1_Configuration();
  GPIO_SWITCH_Configuration();
}

/*--------Key input test--------*/

void KEY_Test(void) 
{
  LED_Off_ALL();
 
  if(GPIO_ReadInputDataBit(GPIO_KEY, GPIO_SWITCH_Y) == Bit_SET) {LED_On_0();}
  else {LED_Off_0();}
  
  if(GPIO_ReadInputDataBit(GPIO_KEY, GPIO_KEY_1) == Bit_SET) {LED_On_1();}
  else {LED_Off_1();}
}

