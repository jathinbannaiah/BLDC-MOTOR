/*
 * L298N H Bridge Driver using PWM
 * Sarvjit Ajit Patil and Praveen Bannaiah
 * Date : 08/08/2023
 */



/*
 * main.c
 */
#include <stdio.h>

#include <stdint.h>

#include <string.h>

#include <stdlib.h>

#include "inc/tm4c123gh6pm.h"

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


extern void PWM_Start(void);


int main(void)
{
    EnableInterrupts() ;

    Systick_init();
//    GPIOA_INIT();
//    GPIOB_INIT();
//    GPIOC_INIT();
//    GPIOD_INIT();
//    GPIOE_INIT();
//    GPIOF_INIT();

//    TIMER0_INIT();

    uart0_init();
//    uart1_init();

    uart0_send_str("Welcome to UART0\n");
    uart0_send_str("This is for debugging.\n");

    BLDC_INIT(0);
    BLDC_INIT_R(0);

    uart0_send_str("outside bldc initialisation\n");

//    Run_FW();

//    delayMs(5000);
//
//    Run_RV();

//    QEI_0_INIT();

//    PWM_0_SYNC() ;

    float set_duty = 0.9 ;
    float set_duty_R = 0.9 ;
/* PWM is used as it triggers the ADC */
    M0PWM4_init(PWM_FRE);
    M0PWM5_init(PWM_FRE);
    M1PWM5_init(PWM_FRE);

    M0PWM4_set(90/100);
    M0PWM4_set(set_duty);

    M0PWM5_set(90/100);
    M0PWM5_set(set_duty_R);

    M1PWM5_set(90/100);
    M1PWM5_set(set_duty_R);

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


    uart0_send_str("going into loop\n");

    while(1)
    {
        if ( rx0_command_flag )
        {
//            uart0_send_str(RX0_BUF);
//            uart0_send_char('\n');

            if(compare_str(RX0_BUF,"FW"))
            {
                Change_DIR(1);
            }

            else if(compare_str(RX0_BUF,"R FW"))
            {
                Change_DIR_R(1);
            }

            else if(compare_str(RX0_BUF,"RV"))
            {
                Change_DIR(0);
            }

            else if(compare_str(RX0_BUF,"R RV"))
            {
                Change_DIR_R(0);
            }

            else if(compare_str(RX0_BUF,"STOP IN"))
            {
                Inst_Stop();
            }

            else if(compare_str(RX0_BUF,"R STOP IN"))
            {
                Inst_Stop_R();
            }

            else if(compare_str(RX0_BUF,"STOP DC"))
            {
                Dec_Stop();
            }

            else if(compare_str(RX0_BUF,"R STOP DC"))
            {
                Dec_Stop_R();
            }

            else if(compare_str(RX0_BUF,"START"))
            {
                Start();
            }

            else if(compare_str(RX0_BUF,"R START"))
            {
                uart0_send_str("CALLING R START FUNCTION\n");

                Start_R();
            }

            else if(compare_str(RX0_BUF,"RUN FW"))
            {
                Run_FW();
            }

            else if(compare_str(RX0_BUF,"R RUN FW"))
            {
                Run_FW_R();
            }

            else if(compare_str(RX0_BUF,"RUN RV"))
            {
                Run_RV();
//                M0PWM4_set(0.5);
//                M0PWM5_set(0.5);
            }

            else if(compare_str(RX0_BUF,"R RUN RV"))
            {
                Run_RV_R();
            }

            else if(compare_str(RX0_BUF,"BRAKE"))
            {
                Brake();
            }

            else if(compare_str(RX0_BUF,"R BRAKE"))
            {
                Brake_R();
            }

            char cmd[3];

            for(int i = 0; i < 3; i++)
            {
                cmd[i] = RX0_BUF[i];
            }

            if(compare_str(cmd,"RPM"))     //Upper case RPM for Re-coater control
            {
                uart0_send_str("RPM\n");
                uart0_send_str(cmd);
                uart0_send_char('\n');

                char rpm[4] = {'\0'};
                for(int i = 0; i < 4; i++)
                {
                    rpm[i] = RX0_BUF[i+4];
                }

                uart0_send_str(rpm);
                uart0_send_char('\n');

                int i_rpm = atoi(rpm);
                sprintf(str,"RPM: %d\n", i_rpm);
                uart0_send_str(str);

                float f_rpm = map(i_rpm,100, 3150, 0.12, 0.97);
//                float f_rpm;
//                f_rpm = i_rpm/100;

                sprintf(str,"RPM(map): %d\n", i_rpm);
                uart0_send_str(str);

                int ii_f_rpm = f_rpm * 100 ;
                int i_f_rpm = ii_f_rpm / 100 ;
                int f_f_rpm = ii_f_rpm % 100 ;

                sprintf(str,"RPM(float): %d.%d\n", i_f_rpm, f_f_rpm);
                uart0_send_str(str);

                M0PWM4_set(f_rpm);

//                for(int i = 0; i < 3; i++)
//                {
//                    RX0_BUF[i] = '\0';
//                    cmd[i] = '\0' ;
//                }
//                for(int i = 0; i < 4; i++)
//                {
//                    RX0_BUF[i] = '\0';
//                    rpm[i] = '\0' ;
//                }

                for(int i = 0; i < 4; i++)
                {
                    rpm[i] = '\0';
                }
                i_rpm = '\0';
                f_rpm = '\0';
            }


            else if(compare_str(cmd,"rpm"))     //Lower case RPM for Re-coater control
            {
                uart0_send_str("RPM Roller Control\n");
                uart0_send_str(cmd);
                uart0_send_char('\n');

                char r[4] = {'\0'};
                for(int j = 0; j < 4; j++)
                {
                    sprintf(str,"jj %c", r[j]);
                    uart0_send_str(str);
                    r[j] = RX0_BUF[j+4];
                    uart0_send_char('\n');
                }

                uart0_send_str(r);
                uart0_send_char('\n');

                int i_rpm_r = atoi(r);
                sprintf(str,"RPM Roller: %d\n", i_rpm_r);
                uart0_send_str(str);

                float f_rpm_r = map(i_rpm_r,100, 3150, 0.12, 0.97);
//                float f_rpm;     M0PWM5_set(f_rpm);
//                f_rpm = i_rpm/100;

                sprintf(str,"RPM(map) Roller: %d\n", i_rpm_r);
                uart0_send_str(str);

                int ii_f_rpm_r = f_rpm_r * 100 ;
                int i_f_rpm_r = ii_f_rpm_r / 100 ;
                int f_f_rpm_r = ii_f_rpm_r % 100 ;

                sprintf(str,"RPM(float) Roller: %d.%d\n", i_f_rpm_r, f_f_rpm_r);
                uart0_send_str(str);

                M1PWM5_set(f_rpm_r);

//                for(int i = 0; i < 3; i++)
//                {
//                    RX0_BUF[i] = '\0';
//                    cmd[i] = '\0' ;
//                }
//                for(int i = 0; i < 4; i++)
//                {
//                    RX0_BUF[i] = '\0';
//                    rpm[i] = '\0' ;
//                }
            }

            for(int i = 0; i < 10; i++)
            {
                RX0_BUF[i] = '0';
            }

            rx0_command_flag = 0 ;
//            M0PWM4_set(0.5);
//            M0PWM5_set(0.5);

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
