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

int temp;

int BLDC_INIT(int mode)
{
    uart0_send_str("Initialising BLDC motor 1\n");
    GPIOB_INIT();
    uart0_send_str("Successfully Initialized Port B\n");

    temp = set_Mode(mode);
    uart0_send_str("Successfully set motor 1 mode\n");

    GPIO_PORTB_DATA_R |= (START | BRAKE);
    return 1;
}

int BLDC_INIT_R(int mode)
{
    uart0_send_str("Initialising BLDC motor 2\n");
    GPIOD_INIT();
    uart0_send_str("Successfully Initialized Port D\n");
    temp = set_Mode_R(mode);
    uart0_send_str("Successfully set motor 2 mode\n");
    GPIO_PORTD_DATA_R |= (START_R | BRAKE_R);

    return 1;

}

void Inst_Stop()
{
    uart0_send_str("Instantaneously stopping Motor 1\n");
//    GPIO_PORTB_DATA_R &= ~ (START)
    GPIO_PORTB_DATA_R |= (BRAKE);
    M0PWM4_stop();
}

void Inst_Stop_R()
{
    uart0_send_str("Instantaneously stopping Motor 2\n");
//    GPIO_PORTB_DATA_R &= ~ (START)
    GPIO_PORTD_DATA_R |= (BRAKE_R);
    M1PWM5_stop();
}

void Dec_Stop()
{
    uart0_send_str("Decelerating stop, Motor 1\n");
    GPIO_PORTB_DATA_R |= (START);
    M0PWM4_stop();

}

void Dec_Stop_R()
{
    uart0_send_str("Decelerating stop, Motor 2\n");
    GPIO_PORTD_DATA_R |= (START_R);
    M1PWM5_stop();
}

void Start()
{
    uart0_send_str("Starting Motor 1\n");
    GPIO_PORTB_DATA_R &= ~ (START | BRAKE);
    M0PWM4_start();

}

void Start_R()
{
    uart0_send_str("Starting Motor 2\n");
    GPIO_PORTD_DATA_R &= ~ (START_R | BRAKE_R);
    M1PWM5_start();
}

void Brake()
{
    uart0_send_str("Stopping Motor 1\n");
    GPIO_PORTB_DATA_R |= (START | BRAKE);
    M0PWM4_stop();
}

void Brake_R()
{
    uart0_send_str("Stopping Motor 2\n");
    GPIO_PORTD_DATA_R |= (START_R | BRAKE_R);
    M1PWM5_stop();
}

void Run_FW()
{
    uart0_send_str("Motor 1: Run Forward \n");
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
    uart0_send_str("Motor 2: Run Forward \n");
    GPIO_PORTD_DATA_R &= ~ (START_R | BRAKE_R);
    GPIO_PORTD_DATA_R |= (DIR_R);
    M1PWM5_start();
}

void Change_DIR(int dir)
{
    uart0_send_str("Motor 1: Change Direction \n");
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
    uart0_send_str("Motor 2: Change Direction \n");
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

int set_Mode(int mode)
{
    switch(mode)
    {
    case 0:
        GPIO_PORTB_DATA_R |=  (M0 | M1) ;
        return 0;
        break;

    case 1:
        GPIO_PORTB_DATA_R &= ~ (M0) ;
        GPIO_PORTB_DATA_R |=  (M1) ;
        return 1;
        break;

    case 2:
        GPIO_PORTB_DATA_R &= ~ (M1) ;
        GPIO_PORTB_DATA_R |=  (M0) ;
        return 2;
        break;

    case 3:
        GPIO_PORTB_DATA_R &= ~ (M0 | M1) ;
        return 3;
        break;

    case 4:
        return -1;
        break;

    case 5:
        return -1;
        break;

    case 6:
        return -1;
        break;


    case 7:
        break;

    }
    return -1;

}

int set_Mode_R(int mode)
{
    switch(mode)
    {
    case 0:
        GPIO_PORTD_DATA_R |=  (M0_R | M1_R) ;
        return 0;
        break;

    case 1:
        GPIO_PORTD_DATA_R &= ~ (M0_R) ;
        GPIO_PORTD_DATA_R |=  (M1_R) ;
        return 1;
        break;

    case 2:
        GPIO_PORTD_DATA_R &= ~ (M1_R) ;
        GPIO_PORTD_DATA_R |=  (M0_R) ;
        return 2;
        break;

    case 3:
        GPIO_PORTD_DATA_R &= ~ (M0_R | M1_R) ;
        return 3;
        break;

    case 4:
        return -1;
        break;

    case 5:
        return -1;
        break;

    case 6:
        return -1;
        break;


    case 7:
        return -1;
        break;

    }
    return -1;

}

