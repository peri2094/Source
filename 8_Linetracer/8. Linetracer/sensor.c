#include "sensor.h"

void sensor_init()
{
	DDRC &= 0xF0;
}

int get_sensor()
{
	return (PINC & 0x0F);
}

int get_error(int val) // 흰 바탕의 검은색 라인 검출
{
	int error = 0;
	int check = 0;
	
	if((val & 0x08) == 0x08) {error += 3; check++;}
	if((val & 0x04) == 0x04) {error += 1; check++;}
	if((val & 0x02) == 0x02) {error -= 1; check++;}
	if((val & 0x01) == 0x01) {error -= 3; check++;}
	
	if(error == 4) {error -= 2;} // error가 4 or -4 (1100, 0011)일경우 error값을 2 or -2로 수정
	if(error == -4) {error += 2;}
	
	if((check == 1)&&(error == -3)) {error = -2;} //(1110, 0111)일 경우 error값을 2 or -2로 수정
	if((check == 1)&&(error == 3)) {error = 2;}
	
	return error;
}


