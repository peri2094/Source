#include "motor.h"

ISR(TIMER0_COMP_vect)
{
	static unsigned int i = 0;
	int j;
	char out_val1[4] = {0x09, 0x0C, 0x06, 0x03};
	
	j = (i++) % 4;
	PORTA = out_val1[j];
}

ISR(TIMER2_COMP_vect)
{
	static unsigned int i = 0;
	int j;
	char out_val2[4] = {0x03, 0x06, 0x0C, 0x09}; 
	
	j = (i++) % 4;
	PORTB = out_val2[j];
}

void motor1_set_spd(int pps1) // pps1, 2의 값을 (기준속도 - 에러값에 따른 오차수정값으로 설정)
{
	if(pps1 <= 0) {TIMSK &= 0xFD;} // 에러발생 시 동작 종료
	else {OCR0 = (int)((15625 / pps1) - 1);}
}

void motor2_set_spd(int pps2)
{
	if(pps2 <= 0) {TIMSK &= 0x6F;}
	else {OCR2 = (int)((15625 / pps2) - 1);}
}

void motor1_init()
{
	cli();
	DDRA |= 0x0F;
	TCCR0 |= 0x1F; // 16M / 1024 = 15625Hz
	TCNT0 = 0x00;
	TIMSK |= 0x02;
	TIFR &= 0xFD;
	ASSR = 0x00;
	sei();
}

void motor2_init()
{
	cli();
	DDRB |= 0x0F;
	TCCR2 |= 0x1D; // 16M / 1024 = 15625Hz
	TCNT2 = 0x00;
	TIMSK |= 0x80;
	TIFR &= 0xEF;
	ASSR = 0x00;
	sei();
}

void motor1_get_spd(int error) // 속도 제어
{
	if(error != 0)  {motor1_set_spd(DEF_SPD - (error * CH_SPD));}
	else {motor1_set_spd(DEF_SPD);}
}

void motor2_get_spd(int error)
{
	if(error != 0) {motor2_set_spd(DEF_SPD + (error * CH_SPD));}
	else{motor2_set_spd(DEF_SPD);}
}


/*

#include "motor.h"

ISR(TIMER0_COMP_vect)
{
	static unsigned int i = 0;
	int j;
	char out_val1[4] = {0x08, 0x04, 0x02, 0x01};
	
	j = (i++) % 4;
	PORTA = out_val1[j];
}

ISR(TIMER2_COMP_vect)
{
	static unsigned int i = 0;
	int j;
	char out_val2[4] = {0x01, 0x02, 0x04, 0x08}; //바퀴 방향 앞으로가게 맞춤
	
	j = (i++) % 4;
	PORTB = out_val2[j];
}

void motor1_init(int pps1)  // pps1, 2의 값을 (기준속도 - 에러값에 따른 오차수정값으로 설정)
{
	OCR0 = (int)((15625 / pps1) - 1);
}

void motor2_init(int pps2)
{
	OCR2 = (int)((15625 / pps2) - 1);
}

void ISR_active()
{
	cli();
	DDRA |= 0x0F;
	TCCR0 |= 0x1F; // 16M / 1024 = 15625Hz
	TCNT0 = 0x00;
	DDRB |= 0x0F;
	TCCR2 |= 0x1D; // 16M / 1024 = 15625Hz
	TCNT2 = 0x00;
	TIMSK |= 0x82;
	TIFR &= 0xED;
	ASSR = 0x00;
	sei();
}
*/