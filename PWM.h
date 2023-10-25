/*
 * PWM.h
 *
 *  Created on: 15-Apr-2023
 * Authors: Sarvjit Ajit Patil and Praveen Bannaiah
 */

#ifndef PWM_H_
#define PWM_H_





#endif /* PWM_H_ */


#include <stdint.h>

#include "inc/tm4c123gh6pm.h"

//void PWM_0_SYNC(void);
//
//void PWM_Start(void);
//
//void PWM_Stop(void);


//Module 1 PWM5

int M1PWM5_init(int freq);

void M1PWM5_start(void);

void M1PWM5_stop(void);

int M1PWM5_set(float dutycycle);

void M1PWM5_set_freq(int freq);

/*Module 0 PWM2*/

void M0PWM2_init(int freq);

 void M0PWM2_start(void);

 void M0PWM2_stop(void);

 void M0PWM2_set(float dutycycle);

 void M0PWM2_set_freq(int freq);

/*Module 0 PWM3*/

void M0PWM3_init(int freq);

 void M0PWM3_start(void);

 void M0PWM3_stop(void);

 void M0PWM3_set(float dutycycle);

 void M0PWM3_set_freq(int freq);

 void M0PWM3_update(int COMPA);

/*Module 0 PWM4*/

int M0PWM4_init(int freq);

 void M0PWM4_start(void);

 void M0PWM4_stop(void);

int M0PWM4_set(float dutycycle);

 void M0PWM4_set_freq(int freq);

/*Module 0 PWM5*/

int M0PWM5_init(int freq);

 void M0PWM5_start(void);

 void M0PWM5_stop(void);

int M0PWM5_set(float dutycycle);

 void M0PWM5_set_freq(int freq);

// inline void PWM_Start(void)
// {
//     PWM0_1_CTL_R |= 0x01 ;
//     PWM0_2_CTL_R |= 0x01 ;
//     PWM0_ENABLE_R |= 0x3C ;
// }

// inline void PWM_Stop(void)
// {
//     PWM0_1_CTL_R &= ~0x01 ;
//     PWM0_2_CTL_R &= ~0x01 ;
//     PWM0_ENABLE_R &= ~0x3C ;
// }
