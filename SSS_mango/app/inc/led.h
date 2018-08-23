#ifndef LED_MAKED_H_
#define LED_MAKED_H_

#include "hw_config.h"

#define GPIO_LED                        GPIOC

#define GPIO_LED_0                      GPIO_PIN_6
#define GPIO_LED_1                      GPIO_PIN_7
#define GPIO_LED_2                      GPIO_PIN_8
#define GPIO_LED_3                      GPIO_PIN_9

void LED_On_0(void);
void LED_On_1(void);
void LED_On_2(void);
void LED_On_3(void);
void LED_On_ALL(void);

void LED_Off_0(void);
void LED_Off_1(void);
void LED_Off_2(void);
void LED_Off_ALL(void);

void LED_OnOffAll_Mult();

void GPIO_LED0_Configuration(void);
void GPIO_LED1_Configuration(void);
void GPIO_LED2_Configuration(void);
void GPIO_LED3_Configuration(void);
void LED_Configuration(void);

void LED_Test(void);

void LED0_Toggle(void);
void LED1_Toggle(void);
void LED2_Toggle(void);

#endif
