/*
 * timer.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef TIMER_H_
#define TIMER_H_





#endif /* TIMER_H_ */

#include <stdint.h>

#include <string.h>

#include "inc/tm4c123gh6pm.h"

void WTIMER3_INIT(void);

void TIMER0_INIT(void);

void set_TIMER0_cnt(long cnt);

long read_TIMER0_cnt(void);

