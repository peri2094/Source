#ifndef SENSOR_H_
#define SENSOR_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void sensor_init(void);
int get_sensor(void);
int get_error(int val);

#endif