#include "delay.h"

/*---------Delay Activation--------*/

void delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void Delay_Active(RCC_ClocksTypeDef* RCC_Clocks)
{
  if (SysTick_Config(RCC_Clocks->SYSCLK_Frequency / 1000))
  { 
    while (1);
  }
}
