#ifndef KEY_MAKED_H_
#define KEY_MAKED_H_

#include "hw_config.h"

#define GPIO_KEY                        GPIOC
#define GPIO_SWITCHX                    GPIOA
#define GPIO_SWITCHY                    GPIOD

#define GPIO_KEY_0                      GPIO_PIN_0
#define GPIO_KEY_1                      GPIO_PIN_1
#define GPIO_KEY_2                      GPIO_PIN_2
#define GPIO_KEY_3                      GPIO_PIN_3

#define GPIO_SWITCH_X                   GPIO_PIN_0
#define GPIO_SWITCH_Y                   GPIO_PIN_2


/*
//-------- Mango-M32 Pin setting--------//
#define GPIO_KEY                        GPIOA
#define GPIO_SWITCH                     GPIOC

#define GPIO_KEY_0                      GPIO_PIN_0
#define GPIO_KEY_1                      GPIO_PIN_1

#define GPIO_SWITCH_X                   GPIO_PIN_4
#define GPIO_SWITCH_Y                   GPIO_PIN_5
*/


void GPIO_KEY0_Configuration(void);
void GPIO_KEY1_Configuration(void);
void GPIO_SWITCH_Configuration(void);
void KEY_Configuration(void);

void KEY_Test(void);

#endif 