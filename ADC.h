/*
 * ADC.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef ADC_H_
#define ADC_H_





#endif /* ADC_H_ */

#include <stdio.h>

#include <stdint.h>

#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"

#define N_ADC_CH_IN 2

void ADC_0_SS_0_INIT(void);

void ADC_0_SS_1_INIT(void);

void ADC_0_SS_3_INIT(void);

void ADC_1_SS_1_INIT(void);

int adc_read(void);

extern volatile int ADC_Voltage, ADC_Current ;

extern float set_Curr_A_ADC , set_Curr_B_ADC ;

//extern volatile float var_A , var_B ;

extern const int half_step_seq[8] ;

extern volatile int sequence ;

