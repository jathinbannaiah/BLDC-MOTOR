/*
 * BLDC.c
 *
 *  Created on: 07-Sep-2023
 * Authors: Sarvjit Ajit Patil and Praveen Bannaiah
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

#ifndef GPIO_H_
#include"GPIO.h"
#endif

#ifndef PWM_H_
#include"PWM.h"
#endif

void BLDC_INIT(int mode)
{
    uart0_send_str("bldc iNIT\n");
    GPIOB_INIT();
    uart0_send_str("After GPIOB_Init\n");

    set_Mode(mode);
    uart0_send_str("after set mode\n");

    GPIO_PORTB_DATA_R |= (START | BRAKE);
}

void BLDC_INIT_R(int mode)
{
    uart0_send_str("bldc roller iNIT  \n");
    GPIOD_INIT();
    uart0_send_str("After GPIOA_Init\n");
    set_Mode_R(mode);
    uart0_send_str("outside set mode r\n");
    GPIO_PORTD_DATA_R |= (START_R | BRAKE_R);
}

void Inst_Stop()
{
    uart0_send_str("instANT stop\n");
//    GPIO_PORTB_DATA_R &= ~ (START)
    GPIO_PORTB_DATA_R |= (BRAKE);
    M0PWM4_stop();
}

void Inst_Stop_R()
{
    uart0_send_str("instant Roller stop\n");
//    GPIO_PORTB_DATA_R &= ~ (START)
    GPIO_PORTD_DATA_R |= (BRAKE_R);
    M1PWM5_stop();
}

void Dec_Stop()
{
    uart0_send_str("Dec stop\n");
    GPIO_PORTB_DATA_R |= (START);
    M0PWM4_stop();

}

void Dec_Stop_R()
{
    uart0_send_str("Dec Roller stop\n");
    GPIO_PORTD_DATA_R |= (START_R);
    M1PWM5_stop();
}

void Start()
{
    uart0_send_str("Start\n");
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE);
    M0PWM4_start();

}

void Start_R()
{
    uart0_send_str("Start Roller\n");
    GPIO_PORTD_DATA_R &= ~ (START_R | BRAKE_R);
    M1PWM5_start();
}

void Brake()
{
    uart0_send_str("stop\n");
    GPIO_PORTB_DATA_R |= (START | BRAKE);
    M0PWM4_stop();
}

void Brake_R()
{
    uart0_send_str("stop Roller\n");
    GPIO_PORTD_DATA_R |= (START_R | BRAKE_R);
    M1PWM5_stop();
}

void Run_FW()
{
    uart0_send_str("run fw\n");
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE | DIR);
    M0PWM4_start();
}

void Run_FW_R()
{
    uart0_send_str("run Roller fw\n");
    GPIO_PORTD_DATA_R &= ~ (START_R | BRAKE_R | DIR_R);
    M1PWM5_start();
}

void Run_RV()
{
    uart0_send_str("Run Recoater\n");
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE);
    GPIO_PORTB_DATA_R |= (DIR);
    M0PWM4_start();
}
void Run_RV_R()
{
    uart0_send_str("Run Roller\n");
    GPIO_PORTD_DATA_R &= ~ (START_R | BRAKE_R);
    GPIO_PORTD_DATA_R |= (DIR_R);
    M1PWM5_start();
}

void Change_DIR(int dir)
{
    Dec_Stop();
    if(dir)
    {
        GPIO_PORTB_DATA_R &= ~ (DIR) ;
    }
    else
    {
        GPIO_PORTB_DATA_R |= (DIR) ;
    }
//    GPIO_PORTB_DATA_R = ~ (dir << 3) ;
    Start();
}

void Change_DIR_R(int dir)
{
    Dec_Stop_R();
    if(dir)
    {
        GPIO_PORTD_DATA_R &= ~ (DIR_R) ;
    }
    else
    {
        GPIO_PORTD_DATA_R |= (DIR_R) ;
    }
//    GPIO_PORTB_DATA_R = ~ (dir << 3) ;
    Start_R();
}

void set_Mode(int mode)
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

void set_Mode_R(int mode)
{
    switch(mode)
    {
    case 0:
        uart0_send_str("case 0\n");
        GPIO_PORTD_DATA_R |=  (M0_R | M1_R) ;
        break;

    case 1:
        uart0_send_str("case 1\n");
        GPIO_PORTD_DATA_R &= ~ (M0_R) ;
        GPIO_PORTD_DATA_R |=  (M1_R) ;
        break;

    case 2:
        GPIO_PORTD_DATA_R &= ~ (M1_R) ;
        GPIO_PORTD_DATA_R |=  (M0_R) ;
        break;

    case 3:
        GPIO_PORTD_DATA_R &= ~ (M0_R | M1_R) ;
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

