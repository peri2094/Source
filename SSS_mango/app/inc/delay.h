#ifndef DELAY_H_
#define DELAY_H_

#include "hw_config.h"

void delay(__IO uint32_t nTime);
void TimingDelay_Decrement();
void Delay_Active(RCC_ClocksTypeDef* RCC_Clocks);

static volatile uint32_t TimingDelay;

#endif
