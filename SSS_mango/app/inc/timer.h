#ifndef TIMER_H_
#define TIMER_H_

#include "hw_config.h"

#define TIM_SERVO               TIM2
#define TIM_MOTORX              TIM3
#define TIM_MOTORY              TIM4

void TIM_Configuration();
void Counter_OnOff();

#endif