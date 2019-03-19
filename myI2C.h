#ifndef myI2C_H_
#define myI2C_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define SLA 0x23
#define SLA_W 0x46
#define SLA_R 0x47


int readOnce();

#endif /* myI2C_H_ */
