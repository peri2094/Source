#include "sensor.h"

/*--------Sensor Input GPIO Configuration--------*/

void Sensor_Configuration()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_SENSOR_OUT1 | GPIO_SENSOR_OUT2 | GPIO_SENSOR_OUT3 | GPIO_SENSOR_OUT4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_SENSOR, &GPIO_InitStructure);
}

/*--------Sensor Value Calculation--------*/

int get_sensor()
{
  int val = 0;
  
  if(GPIO_ReadInputDataBit(GPIO_SENSOR, GPIO_SENSOR_OUT1) == Bit_RESET) {val += 0x08;}
  if(GPIO_ReadInputDataBit(GPIO_SENSOR, GPIO_SENSOR_OUT2) == Bit_RESET) {val += 0x04;}
  if(GPIO_ReadInputDataBit(GPIO_SENSOR, GPIO_SENSOR_OUT3) == Bit_RESET) {val += 0x02;}
  if(GPIO_ReadInputDataBit(GPIO_SENSOR, GPIO_SENSOR_OUT4) == Bit_RESET) {val += 0x01;}
  
  return val;
}

/*--------Error Value Output--------*/

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
	
  if((check == 3)&&(error == -3)) {error = -2;} //(1110, 0111)일 경우 error값을 2 or -2로 수정
  if((check == 3)&&(error == 3)) {error = 2;}
	
  return error;
}


