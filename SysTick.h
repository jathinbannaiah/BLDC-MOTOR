/*
 * SysTick.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_





#endif /* SYSTICK_H_ */

#include <stdint.h>

#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"

#define PWM_FREQ 1000

void Systick_init(void);

unsigned long h_us_stepper_counter, h_us_cnt, ms_counter, sec_counter, min_counter, ms_counter_ssd, ms_counter_led, ms_counter_adc, sec_counter_adc, ms_counter_ultrasonic ;

unsigned long ms_counter_1, ms_counter_2, ms_counter_5, ms_counter_10, ms_counter_20 , ms_counter_50, ms_counter_100, ms_counter_200, ms_counter_500;

//unsigned int sine_count = 0 ;
