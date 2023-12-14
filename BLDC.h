/*
 * BLDC.h
 *
 *  Created on: 07-Sep-2023
 *      Author: Sarvjit
 */

#ifndef BLDC_H_
#define BLDC_H_






//#include <stdint.h>

//#include <string.h>

//#include <stdlib.h>

#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"


#ifndef GPIO_H_
#include"GPIO.h"
#endif

#ifndef PWM_H_
#include"PWM.h"
#endif

#ifndef UART_H_
#include"UART.h"
#endif

#define PWM_FRE 2000

#define START 0x00000080        //PA7

#define BRAKE 0x00000040        //PA6

#define DIR 0x00000020          //PA5

#define M0 0x00000010           //PA4

#define M1 0x00000008           //PA3

#define ALMRST 0x00000004       //PA2

#define R_distance 500

#define t_distance 20

#define PITCH 10                 // Pitch of gears and belt ( distance / No of Revolutions )

#define G_Ratio 10              // Gear Ration: To be multiplied to RPM
/*
 * Roller
 * VH VM VL -> PE4 PWM M0PWM4_init
 * */

/*
 * Recoater
 * VH VM VL -> PE5 PWM M0PWM5_init
 * */
void Recoater_BLDC_INIT(int mode);

void Recoater_Set_RPM(int RPM);

void Recoater_Inst_Stop();

void Recoater_Dec_Stop();

void Recoater_Start();

void Recoater_Brake();

void Recoater_Move_Left();

void Recoater_Move_Right();

void Recoater_Change_DIR(int dir);

void Recoater_set_Mode(int mode);

void Recoater_Go_Left(int rpm);

void Recoater_Go_Right(int rpm);

void calculate_time(int curr_RPM, int slow_rpm, int* T_A, int* T_T);

/* Roller */
void Roller_BLDC_INIT(int mode);

void Roller_Set_RPM(int RPM);

void Roller_Inst_Stop();

void Roller_Dec_Stop();

void Roller_Start();

void Roller_Brake();

void Roller_Run_FW();

void Roller_Run_RV();

void Roller_Change_DIR(int dir);

void Roller_set_Mode(int mode);

//const int R_distance = 500 ;        // Recoater active distance

//const int t_distance = 100 ;         // Distance for which speed should gradually increase/decrease

int T_active, T_tap ;

extern unsigned long ms_counter ;

//char *command[] ;

//void (*command_execute[])(int RPM) ;

int size_c ;

#endif /* BLDC_H_ */

