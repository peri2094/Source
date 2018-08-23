#include "switch.h"
#include "motor.h"

void SWITCH_Configuration()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //Switch X, Y GPIO Input Configuration//
  GPIO_InitStructure.GPIO_Pin = GPIO_SWITCH_X | GPIO_SWITCH_Y;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_SWITCH, &GPIO_InitStructure);
}
