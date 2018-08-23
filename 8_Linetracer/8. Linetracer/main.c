#include "sensor.h"
#include "motor.h"

int main(void)
{
	_delay_ms(3000); // 3초후 동작
	
	int error_val;
	sensor_init();   // 포트C를 센서 입력으로 설정
	motor1_init();
	motor2_init();
	
    while (1) 
    {
		error_val = get_error(get_sensor()); // 에러값 저장 (-3 ~ +3)
		
		motor1_get_spd(error_val);
		motor2_get_spd(error_val);
		
		_delay_ms(1);
    }
	return 0;
}









/*

#include "sensor.h"
#include "motor.h"

#define NOR_SPD 550  // 실제 동작 실험하고 조정
#define CH_SPD 85

int main(void)
{
	int error_val;
	int cnt = 0;
	int spd1 , spd2;
	
	sensor_init(); // 포트C를 센서 입력으로 설정
	usart0_init();
	
	while (1)
	{
		error_val = get_error(get_sensor()); // 에러값 저장 (-3 ~ +3)
		
		if(error_val != 0)  // 라인이 기울어져 있을때
		{
			motor1_init(spd1 = NOR_SPD - (error_val * CH_SPD));
			motor2_init(spd2 = NOR_SPD + (error_val * CH_SPD));
		}
		else
		{
			motor1_init(spd1 = NOR_SPD);
			motor2_init(spd2 = NOR_SPD);
		}
		
		if(cnt == 0)
		{
			ISR_active();
			cnt++;
		}
		
	}
	return 0;
}
*/
