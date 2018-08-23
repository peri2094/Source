#include "timer.h"

//--------Timer Config--------//
void TIM_Configuration()
{
  //--------TIMER 2, 3, 4 CLOCK Input--------//
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  //--------TIMER3, 4 Interrupt each makes Motor1, 2 Step Pulse--------//
  //--------Pulse Clock : 450hz--------//
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  TIM_TimeBaseStructure.TIM_Period = 500 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 20 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM_MOTORX, &TIM_TimeBaseStructure);
  
  TIM_PrescalerConfig(TIM_MOTORX, 23, TIM_PSCReloadMode_Immediate);
  TIM_ClearFlag(TIM_MOTORX, TIM_FLAG_Update);
  TIM_ITConfig(TIM_MOTORX, TIM_IT_Update, ENABLE);
  
  TIM_TimeBaseInit(TIM_MOTORY, &TIM_TimeBaseStructure);
  TIM_PrescalerConfig(TIM_MOTORY, 23, TIM_PSCReloadMode_Immediate);
  TIM_ClearFlag(TIM_MOTORY, TIM_FLAG_Update);
  TIM_ITConfig(TIM_MOTORY, TIM_IT_Update, ENABLE);
  
  //--------TIM1 Interrupt each makes Random Number--------//
  TIM_TimeBaseStructure.TIM_Period = 100 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 20 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  TIM_PrescalerConfig(TIM1, 20, TIM_PSCReloadMode_Immediate);
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  
  //--------TIMER2 CH2 (PA1) makes Servomotor PWM Signal--------//
  TIM_OCInitTypeDef TIM_OCInitStructure;

  TIM_TimeBaseStructure.TIM_Period = 50000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM_SERVO, &TIM_TimeBaseStructure);
  
  //PWM Channel 2 Mode Config//
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 24000;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM_SERVO, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM_SERVO, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM_SERVO, ENABLE);
  TIM_Cmd(TIM_SERVO, ENABLE);
}


void Counter_OnOff()
{
  static uint16_t i = 2;
  
  if((i % 2) == 0) {TIM_Cmd(TIM1, ENABLE);}
  else {TIM_Cmd(TIM1, DISABLE);}
  i++;
}