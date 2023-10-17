/*
 * BLDC.h
 *
 *  Created on: 07-Sep-2023
 * Authors: Sarvjit Ajit Patil and Praveen Bannaiah
 */

#ifndef BLDC_H_
#define BLDC_H_





#endif /* BLDC_H_ */

//#include <stdint.h>

//#include <string.h>

//#include <stdlib.h>

#include "inc/tm4c123gh6pm.h"

//RECOATER MOTOR
#define START 0x00000020      //B5
                                                         //PE4 for PWM
#define BRAKE 0x00000010      //B4

#define DIR 0x00000008        //B3

#define M0 0x00000004         //B2

#define M1 0x00000002         //B1

#define ALMRST 0x00000001     //B0

// ROLLER MOTOR
#define START_R 0x00000001    //D0                      //We can use A port because it is interfering with uart communication
                                                        //Using PF1 for PWM signal (the led on the tiva will glow red becuase pf1 is configured to it)
#define BRAKE_R 0x00000002    //D1
                                                        //Using PF1 to use a different PWM module
#define DIR_R 0x00000004      //D2

#define M0_R 0x00000008       //D3

#define M1_R 0x00000040       //D6

#define ALMRST_R 0x00000080   //D7


//RECOATER FUNCTIONS
void BLDC_INIT(int mode);

void Inst_Stop();

void Dec_Stop();

void Start();

void Brake();

void Run_FW();

void Run_RV();

void Change_DIR(int dir);

void set_Mode(int mode);

//ROLLER FUNCTIONS
void BLDC_INIT_R(int mode);

void Inst_Stop_R();

void Dec_Stop_R();

void Start_R();

void Brake_R();

void Run_FW_R();

void Run_RV_R();

void Change_DIR_R(int dir);

void set_Mode_R(int mode);
