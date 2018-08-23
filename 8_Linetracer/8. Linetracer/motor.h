#ifndef TIMER_H_
#define TIMER_H_

#define F_CPU 16000000UL
#define DEF_SPD 500  // 실제 동작 실험하고 조정
#define CH_SPD 72

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void motor1_init();
void motor2_init();
void motor1_set_spd();
void motor2_set_spd();
void motor1_get_spd(int error);
void motor2_get_spd(int error);

#endif
