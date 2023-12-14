/*
 * GPIO.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */
#include<stdio.h>

#include "GPIO.h"

#include"UART.h"

#include "SysTick.h"

#include"interrupt.h"


/*
 *  DIR -> 1 --> Forward
 *  DIR -> 0 --> Backward
 */

/*
 * EN -> 0 --> Start
 * EN -> 1 --> Stop
 */

int debounce_delay = 2 ;

int debounce_del_C = 0, debounce_del_F = 0;

int debounce_flag_C = 0, debounce_flag_F = 0;

int cool_off_flag_A = 0 ;

int volatile read_DIR = 0, dir = 0, done_flag = 0;

/* PORT Initializations */

/* PA 5-7 -> Limit Switch */

void GPIOA_INIT(void)
{
    /* PA2 -> Buzzer PA3 -> Obstacle */
    SYSCTL_RCGC2_R |= 0x00000001 ;       /* Enable clock to GPIO_A_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000001 ;    /* Enable and provide a clock to GPIO Port_A_ in Run mode */

    while((SYSCTL_PRGPIO_R & 0x01) == 0){}; // Wait for clock to stabilize

    GPIO_PORTA_DIR_R |= 0x000000FC ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTA_DIR_R &= ~ 0x000000E1 ;

//    GPIO_PORTA_LOCK_R = 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTA_AFSEL_R |= 0x00000000 ;  /* Disable alternate function on PORT */

    GPIO_PORTA_AMSEL_R |= 0x00000000 ;  /* Disable analog function on PORT */

    GPIO_PORTA_PCTL_R &= ~ 0xFFFFFF00 ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTA_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTA_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTA_DR8R_R |= 0x000000F0 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTA_ODR_R |= 0x00000008 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTA_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTA_PDR_R |= 0x000000F0 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTA_DEN_R |= 0xFC;       /* enable the GPIO pins for digital function */

    GPIO_PORTA_DATA_R |= 0x00000000 ;

    /* INTERRUPTS on PORT A */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    NVIC_PRI0_R |= (NVIC_PRI0_R & 0xFFFFFF1F) | 0x000000A0 ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x01 ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTA_IS_R &= ~0xFF ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTA_IBE_R &= ~0xFF ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
////    GPIO_PORTA_IBE_R|= 0x08 ;
//
//    GPIO_PORTA_IEV_R &= 0xFF ;      /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//    GPIO_PORTA_IEV_R |= 0xE0 ;
//
//    GPIO_PORTA_IM_R |= 0xE0 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTA_ICR_R |= 0xFF ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}

/* PB 0-3 -> DIR */
/* PB 4-7 -> EN */

void GPIOB_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000002 ;       /* Enable clock to GPIO_B_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000002 ;    /* Enable and provide a clock to GPIO Port_B_ in Run mode */

    while((SYSCTL_PRGPIO_R & 0x02) == 0){}; // Wait for clock to stabilize

    GPIO_PORTB_DIR_R |= 0x000000FC ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTB_DIR_R &= ~0xE0 ;

//    GPIO_PORTB_LOCK_R = 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTB_AFSEL_R &= ~ 0xFFFFFFFC ;  /* Disable alternate function on PORT */

    GPIO_PORTB_AMSEL_R &= ~ 0xFFFFFFFF ;  /* Disable analog function on PORT */

    GPIO_PORTB_PCTL_R &= ~ 0xFFFFFF00 ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTB_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTB_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTB_DR8R_R |= 0x00000FC ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTB_ODR_R |= 0x00000000 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTB_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */

//    GPIO_PORTB_PDR_R |= 0x000000E0 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTB_DEN_R |= 0xFC;       /* enable the GPIO pins for digital function */

    GPIO_PORTB_DATA_R |= 0x00000000 ;

    /* INTERRUPTS on PORT A */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | 0x00008000 ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x00000002 ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTB_IS_R &= ~0xA0 ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTB_IBE_R &= ~0xA0 ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
//
//    GPIO_PORTB_IEV_R &= ~0xA0 ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//
//    GPIO_PORTB_IM_R |= 0x80 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTB_ICR_R |= 0xA0 ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}

void GPIOC_INIT(void)
{
    /* Do no use PC[3:0] */

//    SYSCTL_RCGCGPIO_R |= 0x34;      // Enable clock to GPIO PORTC, PORTE and PORTF
//    GPIO_PORTC_DIR_R |= 0x00;       // Set PC 4-7 as input
//    GPIO_PORTE_DIR_R |= 0x01;       // Set PE 0 as output
//    GPIO_PORTC_DEN_R |= 0xF0;       // Digital Enable PC 4-7
//    GPIO_PORTE_DEN_R |= 0x01;       // Digital Enable PE 0
//    GPIO_PORTC_PUR_R |= 0xF0;       // Enable Pull Up Resistors for input PORTC 4-7
//    //led
//    GPIO_PORTF_DIR_R = 0x0E;        // Enable the GPIO pins for the LED (PF3, 2 1) as output
//    GPIO_PORTF_DEN_R = 0x0E;        // Enable the GPIO pins for digital function

    SYSCTL_RCGC2_R |= 0x00000004 ;       /* Enable clock to GPIO_C_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000004 ;    /* Enable and provide a clock to GPIO Port_C_ in Run mode */

//    GPIO_PORTC_CR_R |= 0xFFFFFFFF ;     /* 1 -> The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written */

    GPIO_PORTC_DIR_R &= ~ 0x30 ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTC_LOCK_R = 0x4C4F434B;     /* unlock commit register */

//    GPIO_PORTC_AFSEL_R |= 0x00 ;  /* Disable alternate function on PORT */

//    GPIO_PORTC_AMSEL_R |= 0x00 ;  /* Disable analog function on PORT */

//    GPIO_PORTC_PCTL_R &= ~ 0xF0 ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTC_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTC_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTC_DR8R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTC_ODR_R |= 0x00 ;    /* 1 -> The corresponding pin is configured as open drain */
//
    GPIO_PORTC_DEN_R |= 0x30;       /* enable the GPIO pins for digital function */

    GPIO_PORTC_PUR_R |= 0x30 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTC_PDR_R |= 0x00 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */



    //GPIO_PORTC_DATA_R |= 0x00 ;

    /* INTERRUPTS on PORT C */

    NVIC_PRI0_R |= (NVIC_PRI0_R & 0xFF1FFFFF) | 0x00A00000 ; /*  priority 5 */

    NVIC_EN0_R |= 0x00000004;        /*  Enable interrupt 2 in NVIC */

    GPIO_PORTC_IS_R &= ~0x30 ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/

    GPIO_PORTC_IBE_R &= ~0x30 ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
                                       1 -> Both edges on the corresponding pin trigger an interrupt */

    GPIO_PORTC_IEV_R &= ~0x30 ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */

    GPIO_PORTC_IM_R |= 0x30 ;       /* 0 -> The interrupt from the corresponding pin is masked.
                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */

    GPIO_PORTC_ICR_R |= 0x30 ;      /* 0 -> The corresponding interrupt is unaffected.
                                       1 -> The corresponding interrupt is cleared */
}

void GPIOD_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000008 ;       /* Enable clock to GPIO_D_ at clock gating control register */

//    SYSCTL_RCGCGPIO_R |= 0x00000008 ;    /* Enable and provide a clock to GPIO Port_D_ in Run mode */

    GPIO_PORTD_DIR_R |= 0xFF ;    /* PD2 -> I | PD3-> O */ /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTD_LOCK_R = 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTD_AFSEL_R &= ~ 0xFFFFFFFF ;  /* Disable alternate function on PORT */

    GPIO_PORTD_AFSEL_R |= 0x00000000 ;  /* Enable alternate function on PORTD2 */

    GPIO_PORTD_AMSEL_R &= ~0xFFFFFFFF ;  /* Disable analog function on PORT */

    GPIO_PORTD_PCTL_R &= ~ 0xFFFFFFFF ; /* Regular digital function */

    GPIO_PORTD_PCTL_R |= 0x00000000 ; /* Alternate digital function -> PD2 */

    /* DRIVE STRENGTH of PORT D */

//    GPIO_PORTD_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTD_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTD_DR8R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */

//    GPIO_PORTD_ODR_R |= 0x00000000 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTD_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTD_PDR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTD_DEN_R |= 0xFF;       /* enable the GPIO pins for digital function */

    GPIO_PORTD_DATA_R |= 0x00 ;

    /* INTERRUPTS on PORT D */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    NVIC_PRI0_R |= (NVID_PRI0_R & 0xFFFFFF1F) | 0x0000000A ; /*  priority 5 */
//
//    NVIC_EN0_R |= (INT_GPIOD - 15) ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTD_IS_R &= ~0xFF ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTD_IBE_R &= ~0xFF ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
//
//    GPIO_PORTD_IEV_R &= ~0xFF ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//
//    GPIO_PORTD_IM_R |= 0x00 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTD_ICR_R |= 0xFF ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}

void GPIOE_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000010 ;       /* Enable clock to GPIO_E_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000010 ;    /* Enable and provide a clock to GPIO Port_E_ in Run mode */

    GPIO_PORTE_DIR_R |= 0x07 ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTE_AFSEL_R &= ~ 0x0F ;  /* Disable alternate function on PORT */
//
//    GPIO_PORTE_AMSEL_R &= ~ 0x0F ;  /* Disable analog function on PORT */
//
//    GPIO_PORTE_PCTL_R &= ~ 0x0F ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTE_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTE_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTE_DR8R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTE_ODR_R |= 0x00000000 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTE_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTE_PDR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTE_DEN_R |= 0x2F;       /* enable the GPIO pins for digital function */

    GPIO_PORTE_DATA_R = 0x00 ;

    /* INTERRUPTS on PORT E */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    NVIE_PRI0_R = (NVIE_PRI0_R & 0xFFFFFF1F) | 0x0000000A ; /*  priority 5 */
//
//    NVIE_EN0_R |= (INT_GPIOE - 15) ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTE_IS_R &= ~0xFF ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTE_IBE_R &= ~0xFF ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
//
//    GPIO_PORTE_IEV_R &= ~0xFF ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//
//    GPIO_PORTE_IM_R |= 0x00 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTE_ICR_R |= 0xFF ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}

void GPIOF_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000020 ;       /* Enable clock to GPIO_F_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000020 ;    /* Enable and provide a clock to GPIO Port_F_ in Run mode */

    GPIO_PORTF_DIR_R |= 0x0000000E ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTF_CR_R |= 0xFFFFFFFF ;     /* 1 -> The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written */

    GPIO_PORTF_AFSEL_R &= ~ 0xFFFFFFFF ;  /* Disable alternate function on PORT */

    GPIO_PORTF_AMSEL_R = 0x00000000 ;  /* Disable analog function on PORT */

    GPIO_PORTF_PCTL_R &= ~ 0xFFFFFFFF ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTF_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTF_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTF_DR8R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */

    GPIO_PORTF_ODR_R |= 0x00000000 ;    /* 1 -> The corresponding pin is configured as open drain */

    GPIO_PORTF_PUR_R |= 0x00000011 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */

    GPIO_PORTF_PDR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTF_DEN_R |= 0xFF;       /* enable the GPIO pins for digital function */

    GPIO_PORTF_DATA_R |= 0x00000000 ;

    /* INTERRUPTS on PORT F */

    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */

    NVIC_PRI7_R |= (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00A00000 ; /*  priority 5 */

    NVIC_EN0_R |= 0x40000000;        /*  Enable interrupt 30 in NVIC */

    GPIO_PORTF_IS_R &= ~0x11 ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/

    GPIO_PORTF_IBE_R &= ~0x11 ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
                                       1 -> Both edges on the corresponding pin trigger an interrupt */

    GPIO_PORTF_IEV_R &= ~0x11 ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */

    GPIO_PORTF_IM_R |= 0x11 ;       /* 0 -> The interrupt from the corresponding pin is masked.
                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */

    GPIO_PORTF_ICR_R |= 0x11 ;      /* 0 -> The corresponding interrupt is unaffected.
                                       1 -> The corresponding interrupt is cleared */
}

void GPIOPortA_Handler(void)
{
    volatile int readback_A;

    GPIO_PORTA_ICR_R |= 0xE0;        /* clear PF4 int */
    readback_A = GPIO_PORTA_ICR_R;    /* a read to force clearing of interrupt flag */
    readback_A = readback_A;
}

extern void set_TIMER0_cnt(long cnt);

char debug_str[100] ;
void GPIOPortB_Handler(void)
{
    volatile int readback_B;
    volatile int _A;

//    _X = (GPIO_PORTB_DATA_R & 0x80) ;
    _A = (GPIO_PORTB_DATA_R & 0x40) ;
//    _B = (GPIO_PORTB_DATA_R & 0x20) ;


    if (GPIO_PORTB_MIS_R & 0x20)
    {

        if( !(_A) )
        {
            read_DIR = 0 ;
//            uart0_send_str("CC\n");
//            sprintf(debug_str,"C=%d D=%d\n", g_cnt, read_DIR);
//            uart0_send_str(debug_str);

        //            Counter-Clockwise
        }
        else if ( (_A) )
        {
            read_DIR = 1 ;
//            uart0_send_str("C\n");
//            sprintf(debug_str,"C=%d D=%d\n", g_cnt, read_DIR);
//            uart0_send_str(debug_str);

        //            Clockwise
        }

        GPIO_PORTB_IM_R &= ~ 0x20 ;
        GPIO_PORTB_IM_R |= 0x80 ;
        GPIO_PORTB_ICR_R |= 0x20;

//        uart0_send_str("B\n");
//        sprintf(debug_str,"X=%d\tA=%d\tB=%d\n", _X, _A, _B);
//        uart0_send_str(debug_str);

    }

    if ( (GPIO_PORTB_MIS_R & 0x80) )
    {

        done_flag = 1 ;
        TIMER0_TAV_R = 0 ;
        dir = read_DIR ;
        dir = dir ;
        GPIO_PORTB_IM_R &= ~ 0x80 ;
        GPIO_PORTB_IM_R |= 0x20 ;
        GPIO_PORTB_ICR_R |= 0x80;

//        uart0_send_str("I\n");

    }



    GPIO_PORTB_ICR_R |= 0xA0;        /* clear PB int */
    readback_B = GPIO_PORTB_ICR_R;    /* a read to force clearing of interrupt flag */
    readback_B = readback_B;
}

void GPIOPortC_Handler(void)
{
//    volatile int readback_C;

    int PORT_C = GPIO_PORTC_DATA_R ;

    if(Recoater_Left & PORT_C)
    {
        /* Left Limit Switch hit*/ /* Move RV */
        uart0_send_str("L1\r");
//        Recoater_Move_Right();
        Recoater_Brake();
//        uart0_send_str("L0\r");
    }

    else if(Recoater_Right & PORT_C)
    {
        /* Right Limit Switch hit*/ /* Move FW */
        uart0_send_str("L2\r");
//        Recoater_Move_Left();
        Recoater_Brake();
//        uart0_send_str("L0\r");
    }

    GPIO_PORTC_ICR_R |= 0x30;        /* clear PC4 int */
}

void GPIOPortF_Handler(void)
{
    volatile int readback_F;

    GPIO_PORTF_ICR_R |= 0x11;        /* clear PF4 int */
    readback_F = GPIO_PORTF_ICR_R;    /* a read to force clearing of interrupt flag */
    readback_F = readback_F;
//    LCD_index = 0 ;
}


