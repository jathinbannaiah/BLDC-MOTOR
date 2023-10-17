/*
 * timer.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"timer.h"

#include"interrupt.h"

#ifndef UART_H_
#include"UART.h"
#endif

int CAP_Complete = 0;

unsigned int CAP_VAL_RISE = 0, CAP_VAL_FALL = 0;

unsigned int old_CAP_VAL_RISE = 0;

void TIMER0_INIT(void)
{

    SYSCTL_RCGC1_R |= 0x00010000 ;

    while((SYSCTL_RCGC1_R & 0x00010000) == 0){};

    SYSCTL_RCGCTIMER_R |= 1;        /* enable clock to Timer Block 0 */

    while((SYSCTL_PRTIMER_R & 0x01) == 0){}; // Wait for clock to stabilize

    SYSCTL_RCGCGPIO_R |= 0x00000002 ;    /* Enable and provide a clock to GPIO Port_B_ in Run mode */

    SYSCTL_RCGC2_R |= 2;            /* enable clock to PORTB */

    while((SYSCTL_PRGPIO_R & 0x02) == 0){}; // Wait for clock to stabilize

    GPIO_PORTB_DIR_R &= ~0x40;      /* make PB6 an input pin */

    GPIO_PORTB_DEN_R |= 0x40;       /* make PB6 as digital pin */

    GPIO_PORTB_AFSEL_R |= 0x40;     /* use PB6 alternate function */

    GPIO_PORTB_PCTL_R &= ~0x0F000000; /* configure PB6 for T0CCP0 */

    GPIO_PORTB_PCTL_R |= 0x07000000;

    TIMER0_CTL_R &= ~ TIMER_CTL_TAEN ;      /* Disable Timer 0 */

    TIMER0_CFG_R |= TIMER_CFG_16_BIT ;      /* 16-bit timer */

    TIMER0_TAMR_R &= ~ 0x00000004 ;  /* Edge-Count mode */


    TIMER0_TAMR_R |= ( TIMER_TAMR_TAMR_CAP ) ;  /* Capture Mode, Up-count */

    TIMER0_TAMATCHR_R = 0x07FF;  /* set the count limit */

    TIMER0_CTL_R &= ~0x0000000C ;    /* Positive Edge */

    TIMER0_CTL_R |= TIMER_CTL_TAEN ;
}

inline void set_TIMER0_cnt(long cnt)
{
    TIMER0_TAV_R = cnt ;

    while ( TIMER0_TAR_R != cnt) {}

}

inline long read_TIMER0_cnt(void)
{
    if (TIMER0_TAR_R > TIMER0_TAMATCHR_R)
    {
        return ( TIMER0_TAMATCHR_R - (0x0000FFFF - TIMER0_TAR_R) ) ;
    }
    else return TIMER0_TAR_R ;
}

void WTIMER3_INIT(void)
{
    /* WT3CCP0 -> PD2 */

//    SYSCTL_RCGC1_R|= SYSCTL_RCGC1_TIMER3 ;

//    GPIO_PORTF_DATA_R &= ~0x0E;
    SYSCTL_RCGCWTIMER_R |= 0x00000008;

    // Disable Wide Timer 3 before making any changes
    WTIMER3_CTL_R &= ~ TIMER_CTL_TAEN;

    // Set the Timer 3 to 32-bit mode and select the prescaler value
    WTIMER3_CFG_R = 0x04 ;

    WTIMER3_TAMR_R = ( TIMER_TAMR_TACMR | TIMER_TAMR_TAMR_CAP | TIMER_TAMR_TACDIR | TIMER_TAMR_TAILD );

    // Configure the Timer 3 Capture mode to capture rising edges
    WTIMER3_CTL_R |= TIMER_CTL_TAEVENT_BOTH;

    WTIMER3_TAILR_R = 400000 ;

    // Enable interrupt for Timer 3 capture events
    WTIMER3_IMR_R |= TIMER_IMR_CAEIM;

    WTIMER3_CTL_R |= TIMER_CTL_TAEN;

    WTIMER3_ICR_R |= 0x04 ;

    NVIC_EN3_R |= 0x00000010 ;

//    NVIC_PRI25_R |= 0x02 ;

}

void WTimer3_A_Handler(void)
{
//    uart_send_str("WTimer Interrupt\n");

//    GPIO_PORTF_DATA_R = 0x0E ;

    while (! ( WTIMER3_MIS_R & TIMER_MIS_CAEMIS ) ) { //uart_send_str("Waiting\n");
}

//    uart_send_int(GPIO_PORTD_DATA_R);
//    uart_send_char('\n');
    if (GPIO_PORTD_DATA_R & 0x00000004)
    {
        CAP_VAL_RISE = WTIMER3_TAR_R ;
//        uart_send_str("R\n");

        WTIMER3_ICR_R |= 0x04 ;

    }
    else if ( ! (GPIO_PORTD_DATA_R & 0x00000004) )
    {
        CAP_VAL_FALL = WTIMER3_TAR_R ;
//        uart_send_str("F\n");

        CAP_Complete = 1 ;
        WTIMER3_ICR_R |= 0x04 ;
    }

    WTIMER3_ICR_R |= 0x04 ;
}
