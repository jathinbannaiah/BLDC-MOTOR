/*
 * ultrasonic.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */


#include"ultrasonic.h"

#include"utils.h"

#include"timer.h"

extern int CAP_Complete ;

extern unsigned int CAP_VAL_RISE, CAP_VAL_FALL;

extern unsigned int old_CAP_VAL_RISE;

void trig_pulse(void)
{
    GPIO_PORTD_DATA_R |= 0x00000008 ;

    delayus(1);

    GPIO_PORTD_DATA_R &= ~ 0x00000008;
}

int get_Distance(void)
{
//    WTIMER3_CTL_R |= TIMER_CTL_TAEN;

    GPIO_PORTD_DATA_R |= 0x00000002 ;

    delayus(10);

    GPIO_PORTD_DATA_R &= ~ 0x00000002;


//    delaydebounce(200);

//    if (ms_counter_ultrasonic < 100)
    unsigned int __distance = 0 ;

    if (CAP_Complete)
    {
        ///////////////////////////////////////////////////////////////
//        uart_send_str("--------------------------------------------\n");
//        uart_send_str("RISING: ");
//        uart_send_int(CAP_VAL_RISE);
//        uart_send_char('\n');
//        uart_send_str("--------------------------------------------\n");
//
//        uart_send_str("--------------------------------------------\n");
//        uart_send_str("Old RISING: ");
//        uart_send_int(old_CAP_VAL_RISE);
//        uart_send_char('\n');
//        uart_send_str("--------------------------------------------\n");

//        int time_period ;
//        if (CAP_VAL_RISE > old_CAP_VAL_RISE)
//        {
//            time_period = ( CAP_VAL_RISE - old_CAP_VAL_RISE ) / ( 16 ) ;
//        }
//        else if (old_CAP_VAL_RISE > CAP_VAL_RISE)
//        {
//            time_period = ( ( CAP_VAL_RISE + 400000 ) - old_CAP_VAL_RISE ) / ( 16) ;
//        }

//        uart_send_str("Time Period: ");
//        uart_send_int( time_period );
//        uart_send_char('\n');
//        ///////////////////////////////////////////////////////////////
//
//        uart_send_str("--------------------------------------------\n");
//        uart_send_str("FALLING: ");
//        uart_send_int(CAP_VAL_FALL);
//        uart_send_char('\n');
        ///////////////////////////////////////////////////////////////

        long total_time ;

        if (CAP_VAL_FALL > CAP_VAL_RISE)
        {
            total_time = ( CAP_VAL_FALL - CAP_VAL_RISE ) / ( 2 * 16 ) ;
        }
        else if (CAP_VAL_RISE > CAP_VAL_FALL)
        {
            total_time = ( ( CAP_VAL_FALL + 400000 ) - CAP_VAL_RISE ) / ( 2 * 16 ) ;
        }

        __distance = ( SOUND_SPEED * total_time ) / 1000000 ;

//        uart_send_str("--------------------------------------------\n");
//        uart_send_str("total_time: ");
//        uart_send_int(total_time);
//        uart_send_char('\n');
//        uart_send_str("--------------------------------------------\n");
//
//        uart_send_str("distance: ");
//        uart_send_int(__distance);
//        uart_send_char('\n');
//        uart_send_str("--------------------------------------------\n");

        old_CAP_VAL_RISE = CAP_VAL_RISE ;

        CAP_VAL_RISE = 0;

        CAP_VAL_FALL = 1;

        CAP_Complete = 0 ;
    }

    else ;

//    WTIMER3_CTL_R &= ~ TIMER_CTL_TAEN;

    return __distance ;
}
