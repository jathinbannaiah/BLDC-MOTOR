/*
 * GPIO.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef GPIO_H_
#define GPIO_H_





#endif /* GPIO_H_ */

#include <stdint.h>

#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"

#include "BLDC.h"

#include "UART.h"

#define Recoater_Left 0x10

#define Recoater_Right 0x20

void GPIOA_INIT(void);

void GPIOB_INIT(void);

void GPIOC_INIT(void);

void GPIOD_INIT(void);

void GPIOE_INIT(void);

void GPIOF_INIT(void);

//unsigned int CAR_DOOR, CAR_ENGINE, SAFETY_BELT ;
//
//int CAR_DOOR_ON = 0, CAR_ENGINE_ON = 0, SAFETY_BELT_ON = 0, OBSTACLE = 0;
//
//int CAR_START_STOP_B = 0;
//
//int debounce_delay = 2 ;
//
//int debounce_del_C = 0, debounce_del_F = 0;
//
//int debounce_flag_C = 0, debounce_flag_F = 0;
