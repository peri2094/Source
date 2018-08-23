#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

#include <stdlib.h>
#include <stdio.h>
#include "stm32f10x.h"


///////////////////////////////////////////////////////////////////
/*--------GPIO Setting--------*/

#define RCC_APB2Periph_GPIO_USART1      RCC_APB2Periph_GPIOA
#define RCC_APB2Periph_GPIO_USART3      RCC_APB2Periph_GPIOB

#define GPIO_PIN_0                      ((uint16_t)0x0001)
#define GPIO_PIN_1                      ((uint16_t)0x0002)
#define GPIO_PIN_2                      ((uint16_t)0x0004)
#define GPIO_PIN_3                      ((uint16_t)0x0008)
#define GPIO_PIN_4                      ((uint16_t)0x0010)
#define GPIO_PIN_5                      ((uint16_t)0x0020)
#define GPIO_PIN_6                      ((uint16_t)0x0040)
#define GPIO_PIN_7                      ((uint16_t)0x0080)
#define GPIO_PIN_8                      ((uint16_t)0x0100)
#define GPIO_PIN_9                      ((uint16_t)0x0200)
#define GPIO_PIN_10                     ((uint16_t)0x0400)
#define GPIO_PIN_11                     ((uint16_t)0x0800)
#define GPIO_PIN_12                     ((uint16_t)0x1000)
#define GPIO_PIN_13                     ((uint16_t)0x2000)
#define GPIO_PIN_14                     ((uint16_t)0x4000)
#define GPIO_PIN_15                     ((uint16_t)0x8000)
#define GPIO_PIN_ALL                    ((uint16_t)0xFFFF)

/*--------clock input--------*/

#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2              ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8              ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3              ((uint32_t)0x00008000)
#define RCC_APB2Periph_TIM15             ((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM16             ((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM17             ((uint32_t)0x00040000)
#define RCC_APB2Periph_TIM9              ((uint32_t)0x00080000)
#define RCC_APB2Periph_TIM10             ((uint32_t)0x00100000)
#define RCC_APB2Periph_TIM11             ((uint32_t)0x00200000)

/*--------system clock setting--------*/ 

#define RCC_CR_HSEON                     ((uint32_t)0x00010000) // Use HSE Clock (12Mhz)
#define RCC_CR_HSERDY                    ((uint32_t)0x00020000)

#define FLASH_ACR_LATENCY_2              ((uint8_t)0x02) // flash access latency setting

#define RCC_CFGR_HPRE_DIV1               ((uint32_t)0x00000000) // SYSCLK(= HCLK) not Divided
#define RCC_CFGR_PPRE2_DIV1              ((uint32_t)0x00000000)
#define RCC_CFGR_PPRE1_DIV2              ((uint32_t)0x00000400) // SYSCLK(= HCLK) Divided by 2
#define RCC_CFGR_PLLSRC_HSE              ((uint32_t)0x00010000) // Use HSE clock (12Mhz) selected as PLL clock
#define RCC_CFGR_PLLMUL6                 ((uint32_t)0x00100000) // PLL input clock = HSE clock * 6
#define RCC_CR_PLLON                     ((uint32_t)0x01000000) // PLL Enable
#define RCC_CR_PLLRDY                    ((uint32_t)0x02000000)
#define RCC_CFGR_SW_PLL                  ((uint32_t)0x00000002) // system clock switch to PLL
#define RCC_CFGR_SWS                     ((uint32_t)0x0000000C)

#define CFGR_SWS_Mask                    ((uint32_t)0x0000000C)    
#define CFGR_PLLMull_Mask                ((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask                 ((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask               ((uint32_t)0x00020000)
#define CFGR_HPRE_Set_Mask               ((uint32_t)0x000000F0)
#define CFGR_PPRE1_Set_Mask              ((uint32_t)0x00000700)
#define CFGR_PPRE2_Set_Mask              ((uint32_t)0x00003800)
#define CFGR_ADCPRE_Set_Mask             ((uint32_t)0x0000C000)


#ifdef __cplusplus
#define __I                               volatile
#else 
#define __I                               volatile const
#endif
#define __O                               volatile                 
#define __IO                              volatile

static __I uint8_t APBAHBPrescTable[16] = {0,0,0,0,1,2,3,4,1,2,3,4,6,7,8,9};
static __I uint8_t ADCPrescTable[4] = {2,4,6,8};


//--------Sudoku Solver System option--------//

#define corr_x                          -26
#define corr_y                          -67

/*--------EXTI KEY Interrupt setting--------*/

#define GPIO_PORTSOURCE_KEY             GPIO_PortSourceGPIOC
#define GPIO_PINSOURCE_KEY1             GPIO_PinSource1
#define GPIO_PINSOURCE_KEY3             GPIO_PinSource3

#define GPIO_EXTI_Line_KEY1             EXTI_Line1
#define GPIO_EXTI_Line_KEY3             EXTI_Line3

#define EXTI_KEY                        GPIOC
#define EXTI_KEY1_PIN                   GPIO_Pin_1
#define EXTI_KEY3_PIN                   GPIO_Pin_3

#define GPIO_PORTSOURCE_SWITCHX         GPIO_PortSourceGPIOA
#define GPIO_PORTSOURCE_SWITCHY         GPIO_PortSourceGPIOD
#define GPIO_PINSOURCE_SWX              GPIO_PinSource0
#define GPIO_PINSOURCE_SWY              GPIO_PinSource2

#define GPIO_EXTI_Line_SWX              EXTI_Line0
#define GPIO_EXTI_Line_SWY              EXTI_Line2

#define EXTI_SWITCHX                    GPIOA
#define EXTI_SWITCHY                    GPIOD
#define EXTI_SWX_PIN                    GPIO_Pin_0
#define EXTI_SWY_PIN                    GPIO_Pin_2


//--------Mango M32 Pin setting--------//
/*
#define GPIO_PORTSOURCE_KEY             GPIO_PortSourceGPIOA
#define GPIO_PINSOURCE_KEY0             GPIO_PinSource0
#define GPIO_PINSOURCE_KEY1             GPIO_PinSource1

#define GPIO_EXTI_Line_KEY0             EXTI_Line0
#define GPIO_EXTI_Line_KEY1             EXTI_Line1

#define EXTI_KEY                        GPIOA
#define EXTI_KEY0_PIN                   GPIO_Pin_0
#define EXTI_KEY1_PIN                   GPIO_Pin_1
*/


//////////////////////////////////////////////////////////////

void System_Information();

void RCC_Configuration();
void NVIC_Configuration();
void EXTI_Configuration();

#endif