/*
 * SysTick.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"SysTick.h"

#include"interrupt.h"

void Systick_init(void)
{
    /* Configure SysTick */
    NVIC_ST_RELOAD_R = 160-1;     /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7;             /* enable SysTick interrupt, use system clock */
    EnableInterrupts();                   /* global enable interrupt */
    return;
}

/* SysTick Handler */

void SysTick_Handler(void)
{
    h_us_stepper_counter += 1 ;
    h_us_cnt += 1 ;
//    ms_counter += 1 ;
//    ms_counter_led += 1 ;

}
