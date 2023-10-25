/*
 * ultrasonic.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_





#endif /* ULTRASONIC_H_ */

#include <stdint.h>

#include <string.h>

#include "inc/tm4c123gh6pm.h"

#define SOUND_SPEED 34000

void trig_pulse(void);

int get_Distance(void);
