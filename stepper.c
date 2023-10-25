/*
 * stepper.c
 *
 *  Created on: 26-Aug-2023
 *      Author: Sarvjit
 */
//#define DEBUG 0

#include"stepper.h"

#include<stdio.h>

#ifndef UART_H_
#include"UART.h"
#endif

const float sine[72] = {0, 0.08716, 0.17365, 0.25882, 0.34202, 0.42262, 0.5, 0.57358, 0.64279, 0.70711, 0.76604, 0.81915, 0.86603, 0.90631, 0.93969, 0.96593, 0.98481, 0.99619, 1, 0.99619, 0.98481, 0.96593, 0.93969, 0.90631, 0.86603, 0.81915, 0.76604, 0.70711, 0.64279, 0.57358, 0.5, 0.42262, 0.34202, 0.25882, 0.17365, 0.08716, 0, -0.08716, -0.17365, -0.25882, -0.34202, -0.42262, -0.5, -0.57358, -0.64279, -0.70711, -0.76604, -0.81915, -0.86603, -0.90631, -0.93969, -0.96593, -0.98481, -0.99619, -1, -0.99619, -0.98481, -0.96593, -0.93969, -0.90631, -0.86603, -0.81915, -0.76604, -0.70711, -0.64279, -0.57358, -0.5, -0.42262, -0.34202, -0.25882, -0.17365, -0.08716} ;

const float cosine[72] = {1, 0.99619, 0.98481, 0.96593, 0.93969, 0.90631, 0.86603, 0.81915, 0.76604, 0.70711, 0.64279, 0.57358, 0.5, 0.42262, 0.34202, 0.25882, 0.17365, 0.08716, 0, -0.08716, -0.17365, -0.25882, -0.34202, -0.42262, -0.5, -0.57358, -0.64279, -0.70711, -0.76604, -0.81915, -0.86603, -0.90631, -0.93969, -0.96593, -0.98481, -0.99619, -1, -0.99619, -0.98481, -0.96593, -0.93969, -0.90631, -0.86603, -0.81915, -0.76604, -0.70711, -0.64279, -0.57358, -0.5, -0.42262, -0.34202, -0.25882, -0.17365, -0.08716, 0, 0.08716, 0.17365, 0.25882, 0.34202, 0.42262, 0.5, 0.57358, 0.64279, 0.70711, 0.76604, 0.81915, 0.86603, 0.90631, 0.93969, 0.96593, 0.98481, 0.99619} ;

char debug_str[100] ;
void update_PWM(int deg, float Vdc)
{
    int Ts = 500 ;

    deg = deg / 5 ;

    while( deg > 71 )
    {
        deg = deg - 72 ;
    }

    int sign_a = 0, sign_b = 0 ;

    int MAX_PWM_1 = PWM0_1_LOAD_R, MAX_PWM_2 = PWM0_2_LOAD_R ;

    int Ta, Tb, T0 ;

    int CMPA_1, CMPB_1, CMPA_2, CMPB_2 ;

    Ta = ( Ts * cosine[deg] ) / Vdc ;

    Tb = ( Ts * sine[deg] ) / Vdc ;

    if( Ta < 0 )
    {
        sign_a = 1 ;
        Ta = (-1) * Ta ;
    }
    else
    {
        sign_a = 0 ;
    }

    if ( Tb < 0 )
    {
        sign_b = 1 ;
        Tb = (-1) * Tb ;
    }
    else
    {
        sign_b = 0 ;
    }
    T0 = Ts - (Ta + Tb) ;

#ifdef DEBUG
    sprintf(debug_str,"Ta = %d\tTb = %d\tT0 = %d \n", Ta, Tb, T0);\
    uart0_send_str(debug_str);
#endif

    if ( sign_b )
    {
//        sign_b = 1 ;
        CMPB_1 = ( MAX_PWM_1 * T0 * 0.25 ) / Ts ;
        CMPA_1 = CMPB_1 + (  MAX_PWM_1 * Tb ) / Ts ;
    }
    else
    {
        CMPA_1 = ( MAX_PWM_1 * T0 * 0.25 ) / Ts ;
        CMPB_1 = CMPA_1 + ( MAX_PWM_1 * Tb ) / Ts ;
    }

    if ( sign_a )
    {
//        sign_a = 1 ;
        CMPB_2 = ( ( sign_b * CMPA_1 ) + ( ( 1 - sign_b ) * CMPB_1 ) ) + ( MAX_PWM_2 * T0 * 0.5 ) / Ts ;
        CMPA_2 = CMPB_2 + (  MAX_PWM_2 * Ta ) / Ts ;
    }
    else
    {
        CMPA_2 = ( ( sign_b * CMPA_1 ) + ( ( 1 - sign_b ) * CMPB_1 ) ) + ( MAX_PWM_2 * T0 * 0.5 ) / Ts ;
        CMPB_2 = CMPA_2 + ( MAX_PWM_2 * Ta ) / Ts ;
    }

    PWM0_1_CMPA_R = CMPA_1 ;
    PWM0_1_CMPB_R = CMPB_1 ;
    PWM0_2_CMPA_R = CMPA_2 ;
    PWM0_2_CMPB_R = CMPB_2 ;

#ifdef DEBUG
    sprintf(debug_str,"PWM0_1_CMPA_R = %ld \n", PWM0_1_CMPA_R);\
    uart0_send_str(debug_str);\
    sprintf(debug_str,"PWM0_1_CMPB_R = %ld \n", PWM0_1_CMPB_R);\
    uart0_send_str(debug_str);\
    sprintf(debug_str,"PWM0_2_CMPA_R = %ld \n", PWM0_2_CMPA_R);\
    uart0_send_str(debug_str);\
    sprintf(debug_str,"PWM0_2_CMPB_R = %ld \n", PWM0_2_CMPB_R);\
    uart0_send_str(debug_str);
#endif

}
