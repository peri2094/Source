#ifndef SENSOR_H_
#define SENSOR_H_

#include "hw_config.h"

#define GPIO_SENSOR                      GPIOC

#define GPIO_SENSOR_OUT1                 GPIO_PIN_4
#define GPIO_SENSOR_OUT2                 GPIO_PIN_5
#define GPIO_SENSOR_OUT3                 GPIO_PIN_10
#define GPIO_SENSOR_OUT4                 GPIO_PIN_11

void Sensor_Configuration(void);
int get_sensor(void);
int get_error(int val);

#endif