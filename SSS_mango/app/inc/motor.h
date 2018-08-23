#ifndef MOTOR_H_
#define MOTOR_H_

#include "hw_config.h"

//--------Development Board Pin setting--------//

#define GPIO_MOTORX                     GPIOB
#define GPIO_MOTORY                     GPIOB
#define GPIO_SERVO                      GPIOA

#define GPIO_MOTORX_STEP                GPIO_PIN_1
#define GPIO_MOTORX_ENABLE              GPIO_PIN_5
#define GPIO_MOTORX_DIR                 GPIO_PIN_6

#define GPIO_MOTORY_STEP                GPIO_PIN_7
#define GPIO_MOTORY_ENABLE              GPIO_PIN_8
#define GPIO_MOTORY_DIR                 GPIO_PIN_9

#define GPIO_HIGH_SPD                   GPIO_PIN_6
#define GPIO_LOW_SPD                    GPIO_PIN_7
#define GPIO_PWM                        GPIO_PIN_1

// MANGO M32 : PWM output : pb7(TIM4 CH2)

/*
//-------- Mango-M32 Pin setting--------//
#define GPIO_MOTORY                     GPIOA
#define GPIO_MOTORX                     GPIOC
#define GPIO_SERVO                      GPIOB

#define GPIO_MOTORY_STEP                GPIO_PIN_4
#define GPIO_MOTORY_ENABLE              GPIO_PIN_5
#define GPIO_MOTORY_DIR                 GPIO_PIN_6

#define GPIO_MOTORX_STEP                GPIO_PIN_2
#define GPIO_MOTORX_ENABLE              GPIO_PIN_1
#define GPIO_MOTORX_DIR                 GPIO_PIN_0

#define GPIO_PWM                        GPIO_PIN_7
#define GPIO_HIGH_SPD                   GPIO_PIN_4
#define GPIO_LOW_SPD                    GPIO_PIN_3
*/

//////////////////////////////

void Motor_Configuration();
void MotorX_PositionControl(int step);
void MotorY_PositionControl(int step);
void Motor_DoubleControl(int step1, int step2);
void Pulse1_Act(int pulse1);
void Pulse2_Act(int pulse2);

void Axis_Control(int xdef, int ydef);
void Axis_Init();
void simple_move(int xdef, int ydef);
void prev_stop();
void Pen_Control(int turn);
void Change_Speed();


void Print_1(int *pa, int *pb);
void Print_2(int *pa, int *pb);
void Print_3(int *pa, int *pb);
void Print_4(int *pa, int *pb);
void Print_5(int *pa, int *pb);
void Print_6(int *pa, int *pb);
void Print_7(int *pa, int *pb);
void Print_8(int *pa, int *pb);
void Print_9(int *pa, int *pb);
void Print_Num(int num, int size);

#endif



