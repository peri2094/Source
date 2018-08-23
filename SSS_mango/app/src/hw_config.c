#include "hw_config.h"
#include "debug.h"

/*--------Print System clock information--------*/

void System_Information(RCC_ClocksTypeDef* RCC_Clocks)
{
  printf("---System Clock setting---\n\n");
  printf("SYSCLK_Frequency : %dHz\n", RCC_Clocks->SYSCLK_Frequency);
  printf("HCLK_Frequency : %dHz\n", RCC_Clocks->HCLK_Frequency);
  printf("PCLK1_Frequency : %dHz\n", RCC_Clocks->PCLK1_Frequency);
  printf("PCLK2_Frequency : %dHz\n", RCC_Clocks->PCLK2_Frequency);
  printf("ADCCLK_Frequency : %dHz\n", RCC_Clocks->ADCCLK_Frequency);
}

/*--------System Clock Config--------*/

void RCC_Configuration()
{
  // Reset Clocks and setting HSI Clocks //
  SystemInit();
  // PCLK = HCLK / 16 = 4.5MHz //
  RCC_PCLK1Config(RCC_HCLK_Div16);
}

/*---------NVIC Config--------*/

void NVIC_Configuration()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Set the Vector Table Location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
  /* Enable the EXTI0 Interrupt - SWX*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI1 Interrupt - SW1*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI2 Interrupt - SWY*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI3 Interrupt - SW3*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*--------EXTI Config--------*/

void EXTI_Configuration()
{
  EXTI_InitTypeDef EXTI_InitStructure;
  RCC->APB2ENR |= RCC_APB2Periph_AFIO;
  
  // Configure GPIO pin as input, Connect EXTI0 Line to GPIO pin //
  GPIO_EXTILineConfig(GPIO_PORTSOURCE_SWITCHX, GPIO_PINSOURCE_SWX);
  
  EXTI_InitStructure.EXTI_Line = GPIO_EXTI_Line_SWX;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure GPIO pin as input, Connect EXTI1 Line to GPIO pin*/
  GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY1);
  
  EXTI_InitStructure.EXTI_Line = GPIO_EXTI_Line_KEY1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure GPIO pin as input, Connect EXTI2 Line to GPIO pin*/
  GPIO_EXTILineConfig(GPIO_PORTSOURCE_SWITCHY, GPIO_PINSOURCE_SWY);
  
  EXTI_InitStructure.EXTI_Line = GPIO_EXTI_Line_SWY;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure GPIO pin as input, Connect EXTI3 Line to GPIO pin*/
  GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY3);
  
  EXTI_InitStructure.EXTI_Line = GPIO_EXTI_Line_KEY3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
}