/*
 * PWM.c
 *
 *  Created on: 15-Apr-2023
 * Authors: Sarvjit Ajit Patil and Praveen Bannaiah
 */

#include <PWM.h>

#ifndef UART_H_
#include"UART.h"
#endif

/* PWM PB4 M0PWM3 */

/*--------------------------------------------M1PWM5--------------------------------------------*/

void M1PWM5_init(int freq)
{
    // Enable the necessary clocks
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R1;  // Enable PWM1
    while((SYSCTL_RCGCPWM_R & SYSCTL_RCGCPWM_R1)==0){};
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable GPIO Port F
    while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R5)==0){};
    SYSCTL_RCC_R |= (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_4);  // Set PWM clock divisor

    // Configure GPIO PF1
    GPIO_PORTF_AFSEL_R |= 0x02;  // Enable alternate function for PF1
    GPIO_PORTF_PCTL_R |= GPIO_PCTL_PF1_M1PWM5;  // Configure PF1 as M1PWM5
    GPIO_PORTF_DEN_R |= 0x02;  // Enable digital functionality for PF1

    // Configure PWM1 Generator 2
    PWM1_2_CTL_R &= ~0x00000001;  // Disable PWM1 Generator 2 during configuration
    PWM1_2_CTL_R |= 0x00000002;   // Configure for down-count mode
    PWM1_2_INTEN_R = (PWM_0_INTEN_TRCNTLOAD | PWM_0_INTEN_INTCNTLOAD);  // Enable interrupts
    PWM1_2_GENB_R = 0x000000B0;  // Configure for PWM down-count mode with immediate updates
    PWM1_2_LOAD_R = (125000 / freq);  // Set the load value for the desired frequency

    float dutycycle_init = 0.5;
    PWM1_2_CMPA_R = (1 - dutycycle_init) * (PWM1_2_LOAD_R);  // Set the initial duty cycle

    // Enable PWM1 Generator 2
    PWM1_2_CTL_R |= 0x00000001;
    PWM1_ENABLE_R |= 0x20;  // Enable PWM output for M1PWM5
}
inline void M1PWM5_start(void)
{
    PWM1_2_CTL_R |= 0x00000001;
    PWM1_ENABLE_R |= 0x20;
}

inline void M1PWM5_stop(void)
{
    PWM1_2_CTL_R &= ~0x00000001;
    PWM1_ENABLE_R &= ~0x20;
}

inline void M1PWM5_set_freq(int freq)
{
    PWM1_2_CTL_R &= ~0x00000001;
    PWM1_ENABLE_R &= ~0x20;
    PWM1_2_LOAD_R = (125000 / freq);
    float dutycycle_init = 0.5;
    PWM1_2_CMPA_R = (1 - dutycycle_init) * PWM1_2_LOAD_R;
    PWM1_ENABLE_R |= 0x20;
    PWM1_2_CTL_R |= 0x00000001;
}

inline void M1PWM5_set(float dutycycle)
{
    PWM1_ENABLE_R &= ~0x20;
    PWM1_2_CTL_R &= ~0x00000001;
    PWM1_2_CMPA_R = (1 - dutycycle) * PWM1_2_LOAD_R - 1;
    PWM1_2_CTL_R |= 0x00000001;
    PWM1_ENABLE_R |= 0x20;
}

/*--------------------------------------------M0PWM3--------------------------------------------*/
void M0PWM2_init(int freq)
{
    //GPIO_PORTE_LOCK_R = 0x4C4F434B;
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0 ;
    while((SYSCTL_RCGCPWM_R & SYSCTL_RCGCPWM_R0)==0){};

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1 ;

    while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R1)==0){};

    SYSCTL_RCC_R |= (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_4) ;

//    GPIO_PORTE_CR_R |= 0x10;
   // SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0 ;
   // SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
    GPIO_PORTB_AFSEL_R |= 0x10 ; // PB4
    GPIO_PORTB_AMSEL_R &= ~0x10 ;
//    GPIO_PORTB_PCTL_R &= ~ 0x000F0000 ;
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB4_M0PWM2; //PB4 M0PWM4
    GPIO_PORTB_DEN_R |= 0x10 ;
    //GPIO_PORTB_DIR_R |= 0x10 ;

    PWM0_1_CTL_R &= ~ 0x00000001 ;
    PWM0_1_CTL_R |= 0x00000002 ;
    PWM0_1_INTEN_R |= ( PWM_1_INTEN_TRCNTLOAD | PWM_1_INTEN_INTCNTLOAD ) ;
    PWM0_1_GENA_R |= 0x000000B0 ;
    //PWM0_2_GENB_R = 0x0000008C ;


    PWM0_1_LOAD_R = (125000/freq) ;

    float dutycycle_init = 0.5 ;

    PWM0_1_CMPA_R = ( 1-dutycycle_init )*(PWM0_1_LOAD_R) ; // //

    /* Interrupt */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    PWM0_INTEN_R |= PWM_INTEN_INTPWM2 ;
//
//    NVIC_PRI3_R |= (NVIC_PRI3_R & 0xFFFFFFF1) | 0x0000000A ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x00001000 ;        /*  Enable interrupt 12 in NVIC */
//
//    PWM0_1_ISC_R |= 0x00000002 ;
//    PWM0_ISC_R |= 0x00000002 ;

    /**/

    PWM0_1_CTL_R |= 0x00000001 ;
    PWM0_ENABLE_R |= 0x04 ;

}

inline void M0PWM2_start(void)
{
    PWM0_ENABLE_R |= 0x04 ;
}

inline void M0PWM2_stop(void)
{
    PWM0_ENABLE_R &= ~0x04 ;
}

inline void M0PWM2_set_freq(int freq)
{
    PWM0_ENABLE_R &= ~ 0x10 ;

    PWM0_1_LOAD_R = (125000/freq) ;

    float dutycycle_init = 0.5 ;

    PWM0_1_CMPA_R = ( 1-dutycycle_init )*(PWM0_1_LOAD_R) ;

    PWM0_ENABLE_R |= 0x04 ;
}

inline void M0PWM2_set(float dutycycle)
{
    PWM0_1_CMPA_R = ( 1-dutycycle )*(PWM0_1_LOAD_R) ; // //
}


/* PWM PB5 M0PWM4 */

/*--------------------------------------------M0PWM4--------------------------------------------*/

void M0PWM3_init(int freq)
{
    //GPIO_PORTE_LOCK_R = 0x4C4F434B;
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0 ;
    while((SYSCTL_RCGCPWM_R & SYSCTL_RCGCPWM_R0)==0){};

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1 ;

    while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R1)==0){};

    SYSCTL_RCC_R |= (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_4) ;

//    GPIO_PORTE_CR_R |= 0x10;
   // SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0 ;
   // SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
    GPIO_PORTB_AFSEL_R |= 0x20 ; // PB5
    GPIO_PORTB_AMSEL_R &= ~0x20 ;
//    GPIO_PORTB_PCTL_R &= ~ 0x000F0000 ;
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_M0PWM3; //PE4 M0PWM4
    GPIO_PORTB_DEN_R |= 0x20 ;
    //GPIO_PORTB_DIR_R |= 0x20 ;

    PWM0_1_CTL_R &= ~ 0x00000001 ;
    PWM0_1_CTL_R |= 0x00000002 ;
    PWM0_1_INTEN_R |= ( PWM_1_INTEN_TRCNTLOAD | PWM_1_INTEN_INTCNTLOAD ) ;
//    PWM0_1_GENA_R |= 0x000000B0 ;
    PWM0_1_GENB_R = 0x000000B0 ;


    PWM0_1_LOAD_R = (125000/freq) - 1 ;

    float dutycycle_init = 0.5 ;

    PWM0_1_CMPA_R = ( 1-dutycycle_init )*(PWM0_1_LOAD_R) ; // //

    /* Interrupt */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    PWM0_INTEN_R |= PWM_INTEN_INTPWM2 ;
//
//    NVIC_PRI3_R |= (NVIC_PRI3_R & 0xFFFFFFF1) | 0x0000000A ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x00001000 ;        /*  Enable interrupt 12 in NVIC */
//
//    PWM0_1_ISC_R |= 0x00000002 ;
//    PWM0_ISC_R |= 0x00000002 ;

    /**/

    PWM0_1_CTL_R |= 0x00000001 ;
    PWM0_ENABLE_R |= 0x08 ;
    PWM0_INVERT_R |= 0x08 ;

}

inline void M0PWM3_start(void)
{
    PWM0_ENABLE_R |= 0x08 ;
}

inline void M0PWM3_stop(void)
{
    PWM0_ENABLE_R &= ~0x08 ;
}

inline void M0PWM3_set_freq(int freq)
{
    PWM0_ENABLE_R &= ~ 0x08 ;

    PWM0_1_LOAD_R = (125000/freq) - 1 ;

    float dutycycle_init = 0.5 ;

    PWM0_1_CMPA_R = ( 1-dutycycle_init )*(PWM0_1_LOAD_R) ;

    PWM0_ENABLE_R |= 0x08 ;
}

inline void M0PWM3_set(float dutycycle)
{
    PWM0_1_CMPA_R = ( 1-dutycycle )*(PWM0_1_LOAD_R) ; // //
}



/* PWM PE4 M0PWM4 */

/*--------------------------------------------M0PWM4--------------------------------------------*/

void M0PWM4_init(int freq)
{
    //GPIO_PORTE_LOCK_R = 0x4C4F434B;
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0 ;
    while((SYSCTL_RCGCPWM_R & SYSCTL_RCGCPWM_R0)==0){};

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;    //GPIO port enable, PORT E is rep as 'SYSCTL_RCGCGPIO_R4

    while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R4)==0){};

    SYSCTL_RCC_R |= (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_4) ;

//    GPIO_PORTE_CR_R |= 0x10;
   // SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0 ;
   // SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
    GPIO_PORTE_AFSEL_R |= 0x10 ; // sets  pin PE4 to be used for its alternate function(PWM)
    GPIO_PORTE_AMSEL_R &= ~0x10 ;
//    GPIO_PORTE_PCTL_R &= ~ 0x000F0000 ;
    GPIO_PORTE_PCTL_R |= GPIO_PCTL_PE4_M0PWM4; //PE4 M0PWM4 (specific alternate function)
    GPIO_PORTE_DEN_R |= 0x10 ;
    //GPIO_PORTE_DIR_R |= 0x10 ;

    PWM0_2_CTL_R &= ~ 0x00000001 ;   //Enable the PWM0 module
    PWM0_2_CTL_R |= 0x00000002 ;
    PWM0_2_INTEN_R |= ( PWM_0_INTEN_TRCNTLOAD | PWM_0_INTEN_INTCNTLOAD ) ;
    PWM0_2_GENA_R |= 0x000000B0 ;       //Settings are configured for PWM generator A, it sets the PWN generation parameters
    //PWM0_2_GENB_R = 0x0000008C ;


    PWM0_2_LOAD_R = (125000/freq) ;

    float dutycycle_init = 0.5 ;

    PWM0_2_CMPA_R = ( 1-dutycycle_init )*(PWM0_2_LOAD_R) - 1 ; // //

    /* Interrupt */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    PWM0_INTEN_R |= PWM_INTEN_INTPWM2 ;
//
//    NVIC_PRI3_R |= (NVIC_PRI3_R & 0xFFFFFFF1) | 0x0000000A ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x00001000 ;        /*  Enable interrupt 12 in NVIC */
//
//    PWM0_2_ISC_R |= 0x00000002 ;
//    PWM0_ISC_R |= 0x00000002 ;

    /**/

    PWM0_2_CTL_R |= 0x00000001 ;
    PWM0_ENABLE_R |= 0x10 ;               //Used to enable the PWM generator

}

inline void M0PWM4_start(void)
{
    PWM0_2_CTL_R |= 0x00000001 ;

    PWM0_ENABLE_R |= 0x10 ;
}

inline void M0PWM4_stop(void)
{
    PWM0_2_CTL_R &= ~ 0x00000001 ;

    PWM0_ENABLE_R &= ~0x10 ;
}

inline void M0PWM4_set_freq(int freq)
{
    PWM0_2_CTL_R &= ~ 0x00000001 ;

    PWM0_ENABLE_R &= ~ 0x10 ;

    PWM0_2_LOAD_R = (125000/freq) ;

    float dutycycle_init = 0.5 ;

    PWM0_2_CMPA_R = ( 1-dutycycle_init )*(PWM0_2_LOAD_R) - 1 ;

    PWM0_ENABLE_R |= 0x10 ;

    PWM0_2_CTL_R |= 0x00000001 ;
}

inline void M0PWM4_set(float dutycycle)
{
//    if( dutycycle == 0 )
//    {
//        PWM0_2_CMPA_R = ( 1-dutycycle )*(PWM0_2_LOAD_R) - 1 ; // //
//
//    }
//    else if( dutycycle == 1 )
//    {
//        PWM0_2_CMPA_R = ( 1-dutycycle )*(PWM0_2_LOAD_R) + 1 ; // //
//    }
//    else
    {
        PWM0_ENABLE_R &= ~ 0x10 ;
        PWM0_2_CTL_R &= ~0x00000001 ;
        PWM0_2_CMPA_R = ( 1-dutycycle )*(PWM0_2_LOAD_R) - 1  ; // //
        PWM0_2_CTL_R |= 0x00000001 ;
        PWM0_ENABLE_R |= 0x10 ;

    }
}



/* PWM PE5 M0PWM5 */

/*--------------------------------------------M0PWM5--------------------------------------------*/

void M0PWM5_init(int freq)
{
    SYSCTL_RCGC0_R |= 0x00100000 ;
    //GPIO_PORTE_LOCK_R = 0x4C4F434B;
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0 ;            //enables pwn module 0
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;          //enables clock to port e
    SYSCTL_RCC_R |= (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_4) ;

//    GPIO_PORTE_CR_R |= 0x10;
   // SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0 ;
   // SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
    GPIO_PORTE_AFSEL_R |= 0x20 ; // E
    //GPIO_PORTE_AMSEL_R = 0x00 ;
//    GPIO_PORTE_PCTL_R &= ~ 0x000F0000 ;
    GPIO_PORTE_PCTL_R |= GPIO_PCTL_PE5_M0PWM5; //PE5 M0PWM5
    GPIO_PORTE_DEN_R |= 0x20 ;           //Enables digital functionalities of PE5
    //GPIO_PORTE_DIR_R |= 0x10 ;

    PWM0_2_CTL_R &= ~ 0x00000001 ;        //Used to disable the pwm generator, this is done to ensure that the generator is disable during configuration
    PWM0_2_CTL_R |= 0x00000002 ;          //Sets bit 1(MODE) to configure the generator from down count mode. In down count mode PWM signal counts down from a specified load value to 0
    PWM0_2_INTEN_R |= ( PWM_0_INTEN_TRCNTLOAD | PWM_0_INTEN_INTCNTLOAD ) ;  //Is configured to enable PWM interrrupts, it enables interrupts when the counter is reloaded and when the counter reaches 0
    PWM0_2_GENB_R |= 0x000000B0 ;
    //PWM0_2_GENB_R = 0x0000008C ;


    PWM0_2_LOAD_R = (125000/freq) ;      //Is used the set the desired frequency

    float dutycycle_init = 0.5 ;

    PWM0_2_CMPA_R = ( 1-dutycycle_init )*(PWM0_2_LOAD_R) ; // //

    /* Interrupt */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    PWM0_INTEN_R |= PWM_INTEN_INTPWM2 ;
//
//    NVIC_PRI3_R |= (NVIC_PRI3_R & 0xFFFFFFF1) | 0x0000000A ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x00001000 ;        /*  Enable interrupt 12 in NVIC */
//
//    PWM0_2_ISC_R |= 0x00000002 ;
//    PWM0_ISC_R |= 0x00000002 ;

    /**/

    PWM0_2_CTL_R |= 0x00000001 ;
    PWM0_ENABLE_R |= 0x20 ;
//    PWM0_INVERT_R |= 0x20 ;

}

inline void M0PWM5_start(void)
{
    PWM0_2_CTL_R |= 0x00000001 ;

    PWM0_ENABLE_R |= 0x20 ;
}

inline void M0PWM5_stop(void)
{
    PWM0_2_CTL_R &= ~0x00000001 ;

    PWM0_ENABLE_R &= ~0x20 ;
}

inline void M0PWM5_set_freq(int freq)
{
    PWM0_ENABLE_R &= ~ 0x20 ;

    PWM0_2_CTL_R &= ~0x00000001 ;

    PWM0_2_LOAD_R = (125000/freq) ;

    float dutycycle_init = 0.5 ;

    PWM0_2_CMPA_R = ( 1-dutycycle_init )*(PWM0_2_LOAD_R) ;

    PWM0_2_CTL_R |= 0x00000001 ;

    PWM0_ENABLE_R |= 0x20 ;
}

inline void M0PWM5_set(float dutycycle)
{
//    if( dutycycle == 0 )
//    {
//        PWM0_2_CMPA_R = ( 1-dutycycle )*(PWM0_2_LOAD_R) - 1 ; // //
//
//    }
//    else if( dutycycle == 1 )
//    {
//        PWM0_2_CMPA_R = ( 1-dutycycle )*(PWM0_2_LOAD_R) + 1 ; // //
//    }
//    else
//    {

    PWM0_ENABLE_R &= ~0x20 ;
    PWM0_2_CTL_R &= ~0x00000001 ;
    PWM0_2_CMPA_R = ( 1-dutycycle )*(PWM0_2_LOAD_R) ; // //
    PWM0_2_CTL_R |= 0x00000001 ;
    PWM0_ENABLE_R |= 0x20 ;


//    }
}

/******************************PWM Interrupt handler********************************/

void PWM_0_G2_Handler(void)
{
//    uart0_send_str("P\n");
    PWM0_2_ISC_R |= 0x00000002 ;
}
