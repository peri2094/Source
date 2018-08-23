#ifndef SWITCH_H_
#define SWITCH_H_

#include "hw_config.h"

#define GPIO_SWITCH                     GPIOC
#define GPIO_SWITCH_X                   GPIO_PIN_4
#define GPIO_SWITCH_Y                   GPIO_PIN_5

void SWITCH_Configuration();

#endif