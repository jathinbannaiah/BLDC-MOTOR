/*
 * BLDC.c
 *
 *  Created on: 07-Sep-2023
 *      Author: Sarvjit
 */

/*
 * All pins have logically inverted operation (Active LOW)
 * when corresponding pin is LOW, it is said to be ACTIVE
 *
 * (M2 is DIR pin)
 *
 * M2       M1      M0      Operation Data
 * OFF      OFF     OFF             0               Speed control through external setting device/ PWM
 * OFF      OFF     ON              1               Speed control - VR1
 * OFF      ON      OFF             2               Digital
 * OFF      ON      ON              3
 * ON       OFF     OFF             4
 * ON       OFF     ON              5
 * ON       ON      OFF             6
 * ON       ON      ON              7
 *
 * */
#include"BLDC.h"

#include<stdio.h>

//char *command[] = {
//
//                   "Roller_FW",
//                   "Roller_RV",
//                   "Roller_STOP_IN",
//                   "Roller_STOP_DC",
//                   "Roller_START",
//                   "Roller_RUN_FW",
//                   "Roller_RUN_RV",
//                   "Roller_BRAKE",
//                   "Roller_RPM",
//                   "Recoater_FW",
//                   "Recoater_RV",
//                   "Recoater_STOP_IN",
//                   "Recoater_STOP_DC",
//                   "Recoater_START",
//                   "Recoater_RUN_FW",
//                   "Recoater_RUN_RV",
//                   "Recoater_BRAKE",
//                   "Recoater_Go_Left",
//                   "Recoater_Go_Right",
//                   "Recoater_RPM"
//
//};
//
//void (*command_execute[])(int RPM) = {
//
//                                      Roller_Change_DIR,            // Roller_FW
//                                      Roller_Change_DIR,            // Roller_RV
//                                      Roller_Inst_Stop,             // Roller_STOP_IN
//                                      Roller_Dec_Stop,              // Roller_STOP_DC
//                                      Roller_Start,                 // Roller_START
//                                      Roller_Run_FW,                // Roller_RUN_FW
//                                      Roller_Run_RV,                // Roller_RUN_RV
//                                      Roller_Brake,                 // Roller_BRAKE
//                                      Roller_Set_RPM,               // Roller_RPM
//                                      Recoater_Change_DIR,          // Recoater_FW
//                                      Recoater_Change_DIR,          // Recoater_RV
//                                      Recoater_Inst_Stop,           // Recoater_STOP_IN
//                                      Recoater_Dec_Stop,            // Recoater_STOP_DC
//                                      Recoater_Start,               // Recoater_START
//                                      Recoater_Move_Left,           // Recoater_RUN_FW
//                                      Recoater_Move_Right,          // Recoater_RUN_RV
//                                      Recoater_Brake,               // Recoater_BRAKE
//                                      Recoater_Go_Left,             // Recoater_Go_Left
//                                      Recoater_Go_Right,            // Recoater_Go_Right
//                                      Recoater_Set_RPM              // Recoater_RPM
//
//};


char str[100] ;

int active_rpm = 120 ;

int slow_rpm = 15 ;

//size_c = sizeof(command)/sizeof(char*);

void Recoater_BLDC_INIT(int mode)
{
    GPIOA_INIT();

    float set_duty = 0.2 ;

//    M0PWM4_init(PWM_FRE);
    M0PWM5_init(PWM_FRE);

//    M0PWM4_set(90/100);
    M0PWM5_set(set_duty);

    Recoater_set_Mode(mode);
    GPIO_PORTA_DATA_R |= (START | BRAKE);

//    size_c = sizeof(command)/sizeof(char*);

    uart1_send_str("BLDC1 Initiated\n");
}

inline void Recoater_Set_RPM(int RPM)
{
    float f_rpm = map(RPM * G_Ratio,50, 3150, 0.12, 0.97);

    M0PWM5_set(f_rpm);
}

inline void Recoater_Inst_Stop()
{
//    GPIO_PORTA_DATA_R &= ~ (START)
    GPIO_PORTA_DATA_R |= (BRAKE);
//    M0PWM4_stop();
    M0PWM5_stop();
}

inline void Recoater_Dec_Stop()
{
    GPIO_PORTA_DATA_R |= (START);
//    M0PWM4_stop();
    M0PWM5_stop();
}

inline void Recoater_Start()
{
    GPIO_PORTA_DATA_R &= ~ (START | BRAKE);
//    M0PWM4_start();
    M0PWM5_start();
}

inline void Recoater_Brake()
{
    GPIO_PORTA_DATA_R |= (START | BRAKE);
//    M0PWM4_stop();
    M0PWM5_stop();
}

inline void Recoater_Move_Left()
{
    GPIO_PORTA_DATA_R &= ~ (START | BRAKE | DIR);
//    M0PWM4_start();
    M0PWM5_start();
}

inline void Recoater_Move_Right()
{
    GPIO_PORTA_DATA_R &= ~ (START | BRAKE);
    GPIO_PORTA_DATA_R |= (DIR);
//    M0PWM4_start();
    M0PWM5_start();
}

inline void Recoater_Change_DIR(int dir)
{
    Recoater_Dec_Stop();
    if(dir)
    {
        GPIO_PORTA_DATA_R &= ~ (DIR) ;
    }
    else
    {
        GPIO_PORTA_DATA_R |= (DIR) ;
    }
//    GPIO_PORTA_DATA_R = ~ (dir << 3) ;
    Recoater_Start();
}

inline void Recoater_Go_Left(int rpm)
{
    active_rpm = rpm;

    calculate_time(slow_rpm, active_rpm, &T_active, &T_tap);

//    sprintf(str,"RPM: %d\n", active_rpm);
//    uart1_send_str(str);
//    sprintf(str,"T_active: %d\n", T_active);
//    uart1_send_str(str);
//    sprintf(str,"T_tap: %d\n", T_tap);
//    uart1_send_str(str);

    Recoater_Brake();

    Recoater_Set_RPM(slow_rpm);

    Recoater_Move_Left();

    ms_counter = 0;

    while( ms_counter < T_tap ){}

    Recoater_Set_RPM(active_rpm);

    //                Recoater_Move_Left();

    ms_counter = 0;

    while( ms_counter < T_active ) {}

    Recoater_Set_RPM(slow_rpm);

    ms_counter = 0;

    while( ms_counter < T_tap ){}

    Recoater_Brake();

    T_active = 0;

    T_tap = 0;

}

inline void Recoater_Go_Right(int rpm)
{

    active_rpm = rpm;

    calculate_time(slow_rpm, active_rpm, &T_active, &T_tap);

//    sprintf(str,"RPM: %d\n", active_rpm);
//    uart1_send_str(str);
//    sprintf(str,"T_active: %d\n", T_active);
//    uart1_send_str(str);
//    sprintf(str,"T_tap: %d\n", T_tap);
//    uart1_send_str(str);

    Recoater_Set_RPM(slow_rpm);

    Recoater_Move_Right();

    ms_counter = 0;

    while( ms_counter < T_tap ){}

    Recoater_Set_RPM(active_rpm);

    ms_counter = 0;

    while( ms_counter < T_active ) {}

    Recoater_Set_RPM(slow_rpm);

    ms_counter = 0;

    while( ms_counter < T_tap ){}

    Recoater_Brake();

    T_active = 0;

    T_tap = 0;

        //                Recoater_Brake();
}

inline void calculate_time(int slow_rpm, int active_RPM, int* T_A, int* T_T)
{
    *T_A = ( R_distance * 60 * 1000 ) / ( PITCH * active_RPM ) ;

    *T_T = ( t_distance * 60 * 1000 ) / ( PITCH * slow_rpm ) ;
}

void Recoater_set_Mode(int mode)
{
    switch(mode)
    {
    case 0:
        GPIO_PORTA_DATA_R |=  (M0 | M1) ;
        break;

    case 1:
        GPIO_PORTA_DATA_R &= ~ (M0) ;
        GPIO_PORTA_DATA_R |=  (M1) ;
        break;

    case 2:
        GPIO_PORTA_DATA_R &= ~ (M1) ;
        GPIO_PORTA_DATA_R |=  (M0) ;
        break;

    case 3:
        GPIO_PORTA_DATA_R &= ~ (M0 | M1) ;
        break;

    case 4:
        break;

    case 5:
        break;

    case 6:
        break;


    case 7:
        break;

    }

}

/* Roller */

void Roller_BLDC_INIT(int mode)
{
    GPIOB_INIT();

    float set_duty = 0.2 ;

    M0PWM4_init(PWM_FRE);
//    M0PWM5_init(PWM_FRE);

//    M0PWM4_set(90/100);
    M0PWM4_set(set_duty);

    Roller_set_Mode(mode);
    GPIO_PORTB_DATA_R |= (START | BRAKE);
    GPIO_PORTB_DATA_R |= 0xFF;

//    size_c = sizeof(command)/sizeof(char*);

    uart1_send_str("Roller BLDC 2 Initiated\n");
}

inline void Roller_Set_RPM(int RPM)
{
    float f_rpm = map(RPM * G_Ratio,50, 3150, 0.12, 0.97);

    M0PWM4_set(f_rpm);
}

inline void Roller_Inst_Stop()
{
//    GPIO_PORTB_DATA_R &= ~ (START)
    GPIO_PORTB_DATA_R |= (BRAKE);
    M0PWM4_stop();
//    M0PWM5_stop();
}

inline void Roller_Dec_Stop()
{
    GPIO_PORTB_DATA_R |= (START);
    M0PWM4_stop();
//    M0PWM5_stop();
}

inline void Roller_Start()
{
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE);
    M0PWM4_start();
//    M0PWM5_start();
}

inline void Roller_Brake()
{
    GPIO_PORTB_DATA_R |= (START | BRAKE);
    M0PWM4_stop();
//    M0PWM5_stop();
}

inline void Roller_Run_FW()
{
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE | DIR);
    M0PWM4_start();
//    M0PWM5_start();
}

inline void Roller_Run_RV()
{
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE);
    GPIO_PORTB_DATA_R |= (DIR);
    M0PWM4_start();
//    M0PWM5_start();
}

inline void Roller_Change_DIR(int dir)
{
    Roller_Dec_Stop();
    if(dir)
    {
        GPIO_PORTB_DATA_R &= ~ (DIR) ;
    }
    else
    {
        GPIO_PORTB_DATA_R |= (DIR) ;
    }
//    GPIO_PORTB_DATA_R = ~ (dir << 3) ;
    Roller_Start();
}

void Roller_set_Mode(int mode)
{
    switch(mode)
    {
    case 0:
        GPIO_PORTB_DATA_R |=  (M0 | M1) ;
        break;

    case 1:
        GPIO_PORTB_DATA_R &= ~ (M0) ;
        GPIO_PORTB_DATA_R |=  (M1) ;
        break;

    case 2:
        GPIO_PORTB_DATA_R &= ~ (M1) ;
        GPIO_PORTB_DATA_R |=  (M0) ;
        break;

    case 3:
        GPIO_PORTB_DATA_R &= ~ (M0 | M1) ;
        break;

    case 4:
        break;

    case 5:
        break;

    case 6:
        break;


    case 7:
        break;

    }

}
