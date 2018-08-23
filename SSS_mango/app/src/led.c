#include "led.h"
#include "delay.h"


/*--------LED (0 ~ 3) On, Off --------*/

void LED_Off_0(void) //KEY(0-3)켜기
{
  //GPIO_LED->BSRR |= GPIO_LED_0;
  GPIO_ResetBits(GPIO_LED, GPIO_LED_0);
}

void LED_Off_1(void)
{
  //GPIO_LED->BSRR |= GPIO_LED_1;
  GPIO_ResetBits(GPIO_LED, GPIO_LED_1);
}

void LED_Off_2(void)
{
  //GPIO_LED->BSRR |= GPIO_LED_2;
  GPIO_ResetBits(GPIO_LED, GPIO_LED_2);
}

void LED_Off_3(void)
{
  //GPIO_LED->BSRR |= GPIO_LED_2;
  GPIO_ResetBits(GPIO_LED, GPIO_LED_3);
}


void LED_On_0(void)//KEY(0-3)끄기
{
  //GPIO_LED->BRR |= GPIO_LED_0;
  GPIO_SetBits(GPIO_LED, GPIO_LED_0);
}

void LED_On_1(void)
{
  //GPIO_LED->BRR |= GPIO_LED_1;
  GPIO_SetBits(GPIO_LED, GPIO_LED_1);
}

void LED_On_2(void)
{
  //GPIO_LED->BRR |= GPIO_LED_2;
  GPIO_SetBits(GPIO_LED, GPIO_LED_2);
}

void LED_On_3(void)
{
  //GPIO_LED->BRR |= GPIO_LED_3;
  GPIO_SetBits(GPIO_LED, GPIO_LED_3);
}


void LED_On_ALL(void) // KEY0~3 모두 켜기
{
  LED_On_0();
  LED_On_1();
  LED_On_2();
  LED_On_3();
}

void LED_Off_ALL(void) // KEY0~3 모두 끄기
{
  LED_Off_0();
  LED_Off_1();
  LED_Off_2();
}

/*--------LED (0 ~ 3) Initialization--------*/

void GPIO_LED0_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef LED0_Set;
  
  LED0_Set.GPIO_Pin = GPIO_LED_0;
  LED0_Set.GPIO_Speed = GPIO_Speed_50MHz;
  LED0_Set.GPIO_Mode =  GPIO_Mode_Out_PP;
  
  GPIO_Init(GPIO_LED, &LED0_Set);
}

void GPIO_LED1_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef LED1_Set;
  
  LED1_Set.GPIO_Pin = GPIO_LED_1;
  LED1_Set.GPIO_Speed = GPIO_Speed_50MHz;
  LED1_Set.GPIO_Mode =  GPIO_Mode_Out_PP;
  
  GPIO_Init(GPIO_LED, &LED1_Set);
}


void GPIO_LED2_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef LED2_Set;
  
  LED2_Set.GPIO_Pin = GPIO_LED_2;
  LED2_Set.GPIO_Speed = GPIO_Speed_50MHz;
  LED2_Set.GPIO_Mode =  GPIO_Mode_Out_PP;
  
  GPIO_Init(GPIO_LED, &LED2_Set);
}

void GPIO_LED3_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef LED3_Set;
  
  LED3_Set.GPIO_Pin = GPIO_LED_3;
  LED3_Set.GPIO_Speed = GPIO_Speed_50MHz;
  LED3_Set.GPIO_Mode =  GPIO_Mode_Out_PP;
  
  GPIO_Init(GPIO_LED, &LED3_Set);
}




void LED_Configuration(void)
{  
  GPIO_LED0_Configuration();
  GPIO_LED1_Configuration();
  GPIO_LED2_Configuration();
  GPIO_LED3_Configuration();
}


void LED_OnOffAll_Mult()
{
  static uint32_t count = 0;
  
  if(count < 30)
  {
    LED_On_0();
    LED_Off_1();
    LED_Off_2();
    delay(100);
    
    LED_Off_0();
    LED_On_1();
    LED_Off_2();
    delay(100);
    
    LED_Off_0();
    LED_Off_1();
    LED_On_2();
    delay(100);
    
    LED_Off_0();
    LED_Off_1();
    LED_Off_2();
    delay(100);
    
    count++;
  }
  else {LED_Off_ALL();}
}



void LED_Test(void)
{
#if 1     //1sec마다 LED 0~3 깜박이기
  static uint32_t val = 0;
  if(val%2 == 0){LED_On_ALL();}
  else {LED_Off_ALL();}
  delay(1000);
  val++;
  
#else     //100ms마다 LED하나씩 깜빡이기   
  LED_OnOffAll_Mult();

#endif
}

void LED0_Toggle(void)
{
  static uint32_t val1 = 0;
   if(val1%2 == 0){LED_On_0();}
  else {LED_Off_0();}
  val1++;
}

void LED1_Toggle(void)
{
  static uint32_t val2 = 0;
   if(val2%2 == 0){LED_On_1();}
  else {LED_Off_1();}
  val2++;
}

void LED2_Toggle(void)
{
  static uint32_t val3 = 0;
  if(val3%2 == 0){LED_On_2();}
  else {LED_Off_2();}
  val3++;
  
}




