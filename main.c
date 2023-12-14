/*
 * Recoater & Roller BLDC
 * Sarvjit Ajit Patil 20862
 * Date : 10/11/2023
 */



/*
 * main.c
 */
#include <stdio.h>

#include <stdint.h>

#include <string.h>

#include <stdlib.h>

#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"

#include"SysTick.h"

#include"PWM.h"

//#ifndef ADC_H_
//#include"ADC.h"
//#endif

#include"GPIO.h"

#ifndef UART_H_
#include"UART.h"
#endif

//#ifndef QEI_H_
//#include "QEI.h"
//#endif

// #include"LCD.h"

// #include"utils.h"

//#ifndef TIMER_H_
//#include"timer.h"
//#endif

#ifndef STEPPER_H_
#include"stepper.h"
#endif

#ifndef BLDC_H_
#include"BLDC.h"
#endif

//#define DEBUG

// #include"ultrasonic.h"


char *command[] = {

                   "Roller_FW",
                   "Roller_RV",
                   "Roller_STOP_IN",
                   "Roller_STOP_DC",
                   "Roller_START",
                   "Roller_RUN_FW",
                   "Roller_RUN_RV",
                   "Roller_BRAKE",
                   "Roller_RPM",
                   "Recoater_FW",
                   "Recoater_RV",
                   "Recoater_STOP_IN",
                   "Recoater_STOP_DC",
                   "Recoater_START",
                   "Recoater_RUN_FW",
                   "Recoater_RUN_RV",
                   "Recoater_BRAKE",
                   "Recoater_Go_Left",
                   "Recoater_Go_Right",
                   "Recoater_RPM"

};

void (*command_execute[])(int RPM) = {

                                      Roller_Change_DIR,            // Roller_FW
                                      Roller_Change_DIR,            // Roller_RV
                                      Roller_Inst_Stop,             // Roller_STOP_IN
                                      Roller_Dec_Stop,              // Roller_STOP_DC
                                      Roller_Start,                 // Roller_START
                                      Roller_Run_FW,                // Roller_RUN_FW
                                      Roller_Run_RV,                // Roller_RUN_RV
                                      Roller_Brake,                 // Roller_BRAKE
                                      Roller_Set_RPM,               // Roller_RPM
                                      Recoater_Change_DIR,          // Recoater_FW
                                      Recoater_Change_DIR,          // Recoater_RV
                                      Recoater_Inst_Stop,           // Recoater_STOP_IN
                                      Recoater_Dec_Stop,            // Recoater_STOP_DC
                                      Recoater_Start,               // Recoater_START
                                      Recoater_Move_Left,           // Recoater_RUN_FW
                                      Recoater_Move_Right,          // Recoater_RUN_RV
                                      Recoater_Brake,               // Recoater_BRAKE
                                      Recoater_Go_Left,             // Recoater_Go_Left
                                      Recoater_Go_Right,            // Recoater_Go_Right
                                      Recoater_Set_RPM              // Recoater_RPM

};

void delayMs(int n);

#define PWM_FRE 2000

extern char RX1_BUF[200], RX0_BUF[200], RX1_CMD[200] ;

extern int rx0_command_flag ;

extern unsigned long ms_counter, h_us_cnt ;

extern int dir, done_flag ;

volatile int ADC_Voltage, ADC_Current ;

extern int ADC_Sel_cnt ;

float pos = 0 ;
long cur_cnt = 0 ;
char str[100] ;

int i_rpm ;

extern void PWM_Start(void);

int main(void)
{
    EnableInterrupts() ;

    Systick_init();
//    GPIOA_INIT();
//    GPIOB_INIT();
    GPIOC_INIT();   /* Limit Switches */
//    GPIOD_INIT();
//    GPIOE_INIT();
//    GPIOF_INIT();

//    TIMER0_INIT();

    uart0_init();
    uart1_init();
//
    uart1_send_str("Welcome to UART1\n");
    uart1_send_str("This is for debugging.\n");


//    Run_FW();

//    delayMs(5000);
//
//    Run_RV();

//    QEI_0_INIT();

//    PWM_0_SYNC() ;

//    PWM_Init(PWM_FRE);


//    float set_duty = 0.9 ;
//
//    M0PWM4_init(PWM_FRE);
//    M0PWM5_init(PWM_FRE);
//
//    M0PWM4_set(90/100);
//    M0PWM4_set(set_duty);

    Recoater_BLDC_INIT(0);
    Roller_BLDC_INIT(0);

    char rpm[4];

    size_c = sizeof(command)/sizeof(char*);

//    M0PWM4_start();
//    M0PWM5_start();
//    PWM_Start();

//    M0PWM4_set(set_duty);
//    M0PWM5_init(PWM_FRE);

//    M0PWM2_set(set_duty);
//    PWM_0_SYNC();

//    M0PWM5_set(set_duty);

//    ADC_0_SS_3_INIT() ;

//    ADC_0_SS_0_INIT();

//    ADC_0_SS_1_INIT();

//    ADC_1_SS_1_INIT();

    i_rpm = 120;

    while(1)
    {
        if ( rx0_command_flag )
        {
            uart1_send_str(RX0_BUF);
            uart1_send_char('\n');


            for(int i = 0; i < sizeof(RX0_BUF); i++)
            {
                if(RX0_BUF[i] == ' ')
                {
                    RX0_BUF[i] = '\0';
                }
            }

            uart1_send_str(RX0_BUF);
            uart1_send_char('\n');

            for(int i = 0; i < size_c; i++)
            {
                if(compare_str(RX0_BUF, command[i]))
                {
                    uart0_send_str("\nOK\n\r");
                    sprintf(str,"Command no: %d\n\r Command: %s\n\r", i, command[i]);
                    uart1_send_str(str);

                    if((i == 8) || (i==19))
                    {
                        for(int i = 0; i < 4; i++)
                        {
                            rpm[i] = RX0_BUF[i+13];
                        }

                        //                uart1_send_str("\nRPM:");
                        //                uart1_send_str(rpm);
                        //                uart1_send_char('\n');

                        i_rpm = atoi(rpm);
                    }

                    (*command_execute[i])(i_rpm);

                    sprintf(str,"Ex%d\n\r", i);
                    uart0_send_str(str);

                }
            }

            for(int i = 0; i < sizeof(rpm); i++)
            {
                rpm[i] = '\0';
            }

            for(int i = 0; i < sizeof(RX0_BUF); i++)
            {
                RX0_BUF[i] = '\0';
            }



            rx0_command_flag = 0 ;

        }
        WaitForInterrupt();

    }
}

void delayMs( int n )
{
    int i, j ;

    for(i = 0; i < n ; i ++)
    {
        for(j = 0; j < 3180; j++);
    }
}
