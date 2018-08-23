#include "motor.h"
#include "timer.h"
#include "key.h"
#include "delay.h"

//--------StepMotor Control by TIMER3, 4 Overflow Interrupt--------//
//--------ServoMotor Control by TIMER2 CH2 PWM Output--------//
//--------Pen Header moves approximately 414mm, 41.4cm by x direction--------//
//--------Pen Header moves approximately 400mm, 40cm by y direction--------//

int pulse1 = 0, pulse2 = 0;
int x = 0, y = 0;
int spd_cons = 10;
int sequenceX, sequenceY, stepx_cnt, stepy_cnt;


//--------StepMotor, ServoMotor GPIO Output Configuration--------//
void Motor_Configuration()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //StepMotorX GPIO Output Configuration//
  GPIO_InitStructure.GPIO_Pin = GPIO_MOTORX_STEP | GPIO_MOTORX_ENABLE | GPIO_MOTORX_DIR;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_MOTORX, &GPIO_InitStructure);
  
  //StepMotorY GPIO Output Configuration//
  GPIO_InitStructure.GPIO_Pin = GPIO_MOTORY_STEP | GPIO_MOTORY_ENABLE | GPIO_MOTORY_DIR;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_MOTORY, &GPIO_InitStructure);
 
  //ServoMotor PWM Output Configuration//
  GPIO_InitStructure.GPIO_Pin = GPIO_PWM;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIO_SERVO, &GPIO_InitStructure);
  
  //Motor Spd Configuration//
  GPIO_InitStructure.GPIO_Pin = GPIO_HIGH_SPD | GPIO_LOW_SPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIO_SERVO, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIO_SERVO, GPIO_LOW_SPD);
  GPIO_SetBits(GPIO_SERVO, GPIO_HIGH_SPD);
}

//--------Low Speed Motor step : 200step * 8(microstep) = 5mm => 0.025mm/step--------// 
//--------HIGH Speed Motor step : 200step * 2(microstep) = 20mm => 0.1mm/step--------//
//--------MotorX Position Control--------//
void MotorX_PositionControl(int step)
{
  sequenceX = 1;
  if(step < 0){ step = -step; }
  else{ GPIO_SetBits(GPIO_MOTORX, GPIO_MOTORX_DIR); }
  
  pulse1 = step;
  TIM_Cmd(TIM3, ENABLE);
}

//--------Motor2 Position Control--------//
void MotorY_PositionControl(int step)
{
  sequenceY = 1;
  if(step < 0){ step = -step; }
  else { GPIO_SetBits(GPIO_MOTORY, GPIO_MOTORY_DIR); }
  
  pulse2 = step;
  TIM_Cmd(TIM4, ENABLE);
}

//--------Motor1, 2 Position Control--------//
void Motor_DoubleControl(int step1, int step2)
{
  MotorX_PositionControl(step1);
  MotorY_PositionControl(step2);
}

//--------Motor1 Pulse produce with TIM3 Interrupt--------//
void Pulse1_Act(int pulse1)
{
  static int i = 0;
  i++;
  
  if(pulse1 < 0) {i = 0; GPIO_ResetBits(GPIO_MOTORX, GPIO_MOTORX_DIR);}
  
  if(i % 2 == 0) 
  { 
    if(i == (pulse1 * 2)) 
    {
      sequenceX = 0;
      GPIO_ResetBits(GPIO_MOTORX, GPIO_MOTORX_STEP);
      GPIO_ResetBits(GPIO_MOTORX, GPIO_MOTORX_DIR);
      i = 0;
      TIM_Cmd(TIM3, DISABLE);
    }
    else {GPIO_ResetBits(GPIO_MOTORX, GPIO_MOTORX_STEP);}
  }
  else  { GPIO_SetBits(GPIO_MOTORX, GPIO_MOTORX_STEP);}
  
  stepx_cnt = i;
}


//--------Motor2 Pulse produce with TIM4 Interrupt--------//
void Pulse2_Act(int pulse2)
{
  static int j = 0;
  j++;
  
  if(pulse2 < 0) {j = 0; GPIO_ResetBits(GPIO_MOTORY, GPIO_MOTORY_DIR);}
  
  if(j % 2 == 0) 
  { 
    if(j == (pulse2 * 2)) 
    {
      sequenceY = 0;
      GPIO_ResetBits(GPIO_MOTORY, GPIO_MOTORY_STEP);
      GPIO_ResetBits(GPIO_MOTORY, GPIO_MOTORY_DIR);
      j = 0;
      TIM_Cmd(TIM4, DISABLE);
    }
    else { GPIO_ResetBits(GPIO_MOTORY, GPIO_MOTORY_STEP); } 
  }
  else { GPIO_SetBits(GPIO_MOTORY, GPIO_MOTORY_STEP); }
  
  stepy_cnt = j;
}

//--------Motor X, Y Axis Control--------//
//--------X : 0 ~ 400, y : 0 ~ 320--------//
void Axis_Control(int xdef, int ydef)
{ 
  int movx = xdef - x; 
  int movy = ydef - y;
  
  x = xdef;
  y = ydef;
  
  if((movx == 0) && (movy != 0)) { MotorY_PositionControl(movy * spd_cons); sequenceX = 0; }
  else if((movx != 0) && (movy == 0)) { MotorX_PositionControl(movx * spd_cons); sequenceY = 0; }
  else if((movx != 0) && (movy != 0)) { Motor_DoubleControl((movx * spd_cons), (movy * spd_cons)); }
  
  while(1) {if((sequenceX == 0) && (sequenceY == 0)) break;}
}

void Axis_Init()
{
  x = 0; y = 0;
}

void simple_move(int xdef, int ydef)
{
  if((xdef == 0) && (ydef != 0)) { MotorY_PositionControl(ydef * spd_cons); sequenceX = 0; }
  else if((xdef != 0) && (ydef == 0)) { MotorX_PositionControl(xdef * spd_cons); sequenceY = 0; }
  else if((xdef != 0) && (ydef != 0)) { Motor_DoubleControl((xdef * spd_cons), (ydef * spd_cons)); }
}

void prev_stop()
{
  TIM_Cmd(TIM3, DISABLE);
  TIM_Cmd(TIM4, DISABLE);
  Pulse1_Act(-1); 
  Pulse2_Act(-1);
}

//--------Pen Header Servo Control--------//
void Pen_Control(int turn)
{ 
  delay(100);
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  //1 : Pen Header down, else : Pen Header up//
  if(turn == 1) {TIM_OCInitStructure.TIM_Pulse = 34000;}
  else {TIM_OCInitStructure.TIM_Pulse = 24000;}
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  if(turn == 1) {delay(300);}
  else {delay(150);}
}

//--------Change StepMotor XY Speed : Line check -> Number scan--------//
void Change_Speed()
{
  if(GPIO_ReadInputDataBit(GPIO_SERVO, GPIO_LOW_SPD) == Bit_RESET) 
  {
    GPIO_ResetBits(GPIO_SERVO, GPIO_HIGH_SPD);
    GPIO_SetBits(GPIO_SERVO, GPIO_LOW_SPD);
    spd_cons = 20;
  }
  
  
  else if((GPIO_ReadInputDataBit(GPIO_SERVO, GPIO_HIGH_SPD) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIO_SERVO, GPIO_LOW_SPD) == Bit_SET))
  {
    GPIO_SetBits(GPIO_SERVO, GPIO_HIGH_SPD);
    GPIO_SetBits(GPIO_SERVO, GPIO_LOW_SPD);
    spd_cons = 40;
  }
  
  else
  {
    GPIO_SetBits(GPIO_SERVO, GPIO_HIGH_SPD);
    GPIO_ResetBits(GPIO_SERVO, GPIO_LOW_SPD);
    spd_cons = 10;
  }
}

//--------Print Number to move X, Y axis and Pen header--------//
void Print_Num(int num, int size)
{
  int set_sizeX = size / 7;
  int set_sizeY = size / 4;
  int currentX = x;
  int currentY = y;
  int PositionX[3] = {currentX - set_sizeX, currentX, currentX + set_sizeX};
  int PositionY[3] = {currentY - set_sizeY, currentY, currentY + set_sizeY};
  
  if(num == 1)
  {Print_1(PositionX, PositionY);}
  else if(num == 2)
  {Print_2(PositionX, PositionY);}
  else if(num == 3)
  {Print_3(PositionX, PositionY);}
  else if(num == 4)
  {Print_4(PositionX, PositionY);}
  else if(num == 5)
  {Print_5(PositionX, PositionY);}
  else if(num == 6)
  {Print_6(PositionX, PositionY);}
  else if(num == 7)
  {Print_7(PositionX, PositionY);}
  else if(num == 8)
  {Print_8(PositionX, PositionY);}
  else if(num == 9)
  {Print_9(PositionX, PositionY);}  
}

//--------Print Number 1 to 9--------//
void Print_1(int *pa, int *pb)
{
  Axis_Control(pa[1] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[1] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}

void Print_2(int *pa, int *pb)
{
  Axis_Control(pa[0], pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2], pb[2]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2], pb[1]);
  Pen_Control(0);
  Axis_Control(pa[0], pb[1]);
  Pen_Control(1);
  Axis_Control(pa[2], pb[1]);
  Pen_Control(0);
  Axis_Control(pa[0], pb[1]);
  Pen_Control(1);
  Axis_Control(pa[0], pb[0]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2], pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1], pb[1]);
}
                         
void Print_3(int *pa, int *pb)
{
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[1]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[0]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}

void Print_4(int *pa, int *pb)
{
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[1]);
  Pen_Control(0);
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}                         

void Print_5(int *pa, int *pb)
{
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[1]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
  
}

void Print_6(int *pa, int *pb)
{
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[1]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[0]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}

void Print_7(int *pa, int *pb)
{
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}

void Print_8(int *pa, int *pb)
{
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[1]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[0]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}

void Print_9(int *pa, int *pb)
{
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[0] , pb[1]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[1]);
  Pen_Control(0);
  Axis_Control(pa[0] , pb[2]);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[2]);
  Pen_Control(0);
  Pen_Control(1);
  Axis_Control(pa[2] , pb[0]);
  Pen_Control(0);
  Axis_Control(pa[1] , pb[1]);
}
    