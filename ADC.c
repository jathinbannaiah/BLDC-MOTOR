 /*
 * ADC.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"ADC.h"

#ifndef UART_H_
#include"UART.h"
#endif

char debug_str[100] ;
/* ADC0 -> PE3 */

volatile int ADC_Sel_cnt = 0 ;
/* PE 1 (ADC1 SS1) -> B */
/* PE 2 (ADC0 SS1) -> A */
/* ADC 0 Digital Comparator AIN2(PE1)SS0 and AIN1(PE2)SS1 */

/* Phase B */
/* ADC1 SS1 (PE1/AIN2) with PWM 0 as trigger */
void ADC_1_SS_1_INIT(void)
{
    /*ADC0 -> AIN1 PE2 */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R1 ;

    GPIO_PORTE_AFSEL_R |= 0x00000002 ; //PE1
    GPIO_PORTE_DEN_R &= ~ 0x00000002 ;
    GPIO_PORTE_AMSEL_R |= 0x00000002 ;

    ADC1_ACTSS_R &= ~ 0x00000002 ;

    ADC1_EMUX_R |= 0x00000080 ;

    ADC1_TSSEL_R &= ~ 0x00000030 ;      /* Use Generator 2 (and its trigger) in PWM module 0 */

    ADC1_SSMUX1_R = 0x00000002 ;        /*AIN2*/

    ADC1_SSCTL1_R |= 0x00000040 ;       /* SS1 END on 1st sample */

    ADC1_SSDC1_R |=  0x00000020 ;       /* COMP 2 */

    ADC1_SSOP1_R |= 0x00000010 ;        /* 1st sample is sent to COMP 2 */

//    ADC1_DCRIC_R |= 0x00000002 ;

    ADC1_DCCTL2_R |= 0x0000001C ;       /* Always Enable Interrupt on High band region */

    ADC1_DCCMP2_R =  ( (int)((0.9 * 4095) / 3.3) << 16 ) ;

//    ADC1_DCCMP1_R = ( (int)((set_Curr_B * 4095) / 3.3) * 65536 ) |  (int)((set_Curr_B * 4095) / 3.3) ;

    /* Interrupt */

    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */

    ADC1_IM_R |= (ADC_IM_DCONSS1) ;

//    ADC1_IM_R |= (ADC_IM_MASK1 | ADC_IM_DCONSS1) ;
//    ADC1_IM_R |= (ADC_IM_MASK1) ;

    ADC1_ISC_R |= ( ADC_IM_DCONSS1) ;

//    ADC1_ISC_R |= (ADC_IM_MASK1 | ADC_IM_DCONSS1) ;
//    ADC1_ISC_R |= (ADC_IM_MASK1) ;

    NVIC_PRI11_R |= (NVIC_PRI11_R & 0xFFFF1FFF) | 0x0000F000 ; /*  priority 5 */

    NVIC_EN1_R |= 0x00020000 ;        /*  Enable interrupt 15 in NVIC */

    ADC1_ACTSS_R |= 0x00000002 ;        /* SS0 & SS1 */

    uart0_send_str("ADC1 SS1 Initiated...\n");
}


void ADC_0_SS_0_INIT(void)
{
    /*ADC0 -> AIN2 PE1*/
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0 ;

    GPIO_PORTE_AFSEL_R |= 0x00000002 ; //PE1
    GPIO_PORTE_DEN_R &= ~ 0x00000002 ;
    GPIO_PORTE_AMSEL_R |= 0x00000002 ;

    ADC0_ACTSS_R &= ~ 0x00000001 ;

    ADC0_EMUX_R |= 0x00000008 ;

    ADC0_TSSEL_R &= ~ 0x00300000 ;      /* Use Generator 2 (and its trigger) in PWM module 0 */

    ADC0_SSMUX0_R = 0x00000002 ;        /*AIN2*/

    ADC0_SSCTL0_R |= 0x04 ;             /* SS0 */

    ADC0_SSDC0_R &= ~ 0x0000000F ;      /* COMP 0 */

    ADC0_SSOP0_R |= 0x00000001 ;

    ADC0_DCRIC_R |= 0x00000001 ;

    ADC0_DCCTL0_R |= 0x0000001C ;

    ADC0_DCCMP0_R = 0x08010800 ;

//    ADC0_DCCMP0_R = ( (int)((0.900 * 4095) / 3.3) << 16 ) ; //|  (int)((set_Curr_A * 4095) / 3.3) ;

    /* Interrupt */

    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */

    ADC0_IM_R |= (ADC_IM_DCONSS0) ;

    ADC0_IM_R |= (ADC_IM_MASK0 | ADC_IM_DCONSS0) ;

    ADC0_ISC_R |= (ADC_IM_DCONSS0) ;

    ADC0_ISC_R |= (ADC_IM_MASK0 | ADC_IM_DCONSS0) ;

//    NVIC_PRI3_R |= (NVIC_PRI3_R & 0xFF1FFFFF) | 0x00800000 ; /*  priority 5 */

    NVIC_EN0_R |= 0x00004000 ;        /*  Enable interrupt 14 in NVIC */

    ADC0_ACTSS_R |= 0x00000001 ;        /* Enable SS0 */

    uart0_send_str("ADC SS0 Initiated...\n");
}

/* Phase A */
/* ADC0 SS1 (PE2/AIN1) with PWM 0 as trigger */
void ADC_0_SS_1_INIT(void)
{
    /*ADC0 -> AIN1 PE2 */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0 ;

    GPIO_PORTE_AFSEL_R |= 0x00000004 ; //PE2
    GPIO_PORTE_DEN_R &= ~ 0x00000004 ;
    GPIO_PORTE_AMSEL_R |= 0x00000004 ;

    ADC0_ACTSS_R &= ~ 0x00000002 ;

    ADC0_EMUX_R |= 0x00000080 ;

    ADC0_TSSEL_R &= ~ 0x00000030 ;      /* Use Generator 2 (and its trigger) in PWM module 0 */

    ADC0_SSMUX1_R = 0x00000001 ;        /*AIN1*/

    ADC0_SSCTL1_R |= 0x40 ;             /* SS1 END on 1st sample */

    ADC0_SSDC1_R |=  0x00000010 ;       /* COMP 1 */

    ADC0_SSOP1_R |= 0x00000010 ;        /* 1st sample is sent to COMP 1 */

//    ADC0_DCRIC_R |= 0x00000002 ;

    ADC0_DCCTL1_R |= 0x0000001C ;       /* Always Enable Interrupt on High band region */

    ADC0_DCCMP1_R =  ( (int)((0.9 * 4095) / 3.3) << 16 ) ;

//    ADC0_DCCMP1_R = ( (int)((set_Curr_B * 4095) / 3.3) * 65536 ) |  (int)((set_Curr_B * 4095) / 3.3) ;

    /* Interrupt */

    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */

    ADC0_IM_R |= (ADC_IM_DCONSS1) ;

//    ADC0_IM_R |= (ADC_IM_MASK1 | ADC_IM_DCONSS1) ;
//    ADC0_IM_R |= (ADC_IM_MASK1) ;

    ADC0_ISC_R |= ( ADC_IM_DCONSS1) ;

//    ADC0_ISC_R |= (ADC_IM_MASK1 | ADC_IM_DCONSS1) ;
//    ADC0_ISC_R |= (ADC_IM_MASK1) ;

    NVIC_PRI3_R |= (NVIC_PRI3_R & 0x1FFFFFFF) | 0x80000000 ; /*  priority 5 */

    NVIC_EN0_R |= 0x00008000 ;        /*  Enable interrupt 15 in NVIC */

    ADC0_ACTSS_R |= 0x00000002 ;        /* SS0 & SS1 */

    uart0_send_str("ADC SS1 Initiated...\n");
}

/* ADC 0 SS 3 PE3 with PWM 0 as trigger */
void ADC_0_SS_3_INIT(void)
{
    /*ADC0 -> PE3*/
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0 ;
    GPIO_PORTE_AFSEL_R |= 0x00000008 ; //PE3
    GPIO_PORTE_DEN_R &= ~ 0x00000008 ;
    GPIO_PORTE_AMSEL_R |= 0x00000008 ;

    ADC0_ACTSS_R &= ~ 0x00000008 ;

    ADC0_EMUX_R |= 0x00008000 ;

    ADC0_TSSEL_R &= ~ 0x00300000 ;      /* Use Generator 2 (and its trigger) in PWM module 0 */

    /*ADC0 -> PE2*/
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ;
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0 ;
    GPIO_PORTE_AFSEL_R |= 0x00000004 ; //PE2
    GPIO_PORTE_DEN_R &= ~ 0x00000004 ;
    GPIO_PORTE_AMSEL_R |= 0x00000004 ;

    ADC0_ACTSS_R &= ~ 0x00000008 ;

    ADC0_EMUX_R |= 0x00000800 ;

    ADC0_TSSEL_R &= ~ 0x00300000 ;      /* Use Generator 2 (and its trigger) in PWM module 0 */


    ADC0_SSMUX3_R = 0 ;

    ADC0_SSCTL3_R |= 6 ;


    /* Interrupt */

    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */

    ADC0_IM_R |= ADC_IM_MASK3 ;

    ADC0_ISC_R |= ADC_IM_MASK3 ;

    NVIC_PRI4_R |= (NVIC_PRI4_R & 0xFFFF1FFF) | 0x00008000 ; /*  priority 5 */

    NVIC_EN0_R |= 0x00020000 ;        /*  Enable interrupt 17 in NVIC */

    ADC0_ACTSS_R |= 0x00000008 ;

}


int adc_read(void)
{
    ADC0_PSSI_R |= 0x08 ;
    while ( !(ADC0_RIS_R & ADC_RIS_INR3) ) {}
    int ADC_DATA = ADC0_SSFIFO3_R ;
    ADC0_ISC_R |= 0x00000008 ;
    return ADC_DATA ;
}

void ADC_0_SS3_Handler(void)
{
//    uart0_send_char('0');

//    ADC_Sel_cnt += 1 ;
//    sprintf(debug_str,"ADC = %ld\n", ADC0_SSFIFO3_R);
//    uart0_send_str(debug_str);
//
//    if (ADC_Sel_cnt > (N_ADC_CH_IN - 1) )
//    {
//        ADC_Sel_cnt = 0 ;
//    }
//    else
//    {
//    }
//
//    switch(ADC_Sel_cnt)
//    {
//        case 0:
//            ADC_Voltage = ADC0_SSFIFO3_R ;
//            ADC0_SSMUX3_R = 0 ;     /* Select AIN1 for next sample */
////            sprintf(debug_str,"ADC = %d\n", ADC0_SSFIFO3_R);
////            uart0_send_str(debug_str);
//            break;
//        case 1:
//            ADC_Current = ADC0_SSFIFO3_R ;
//            ADC0_SSMUX3_R = 1 ;     /* Select AIN0 for next sample */
////            sprintf(debug_str,"ADC1 = %ld\n", ADC0_SSFIFO3_R);
////            uart0_send_str(debug_str);
//            break;
//
//    }
//
//
//    ADC0_ISC_R |= ADC_IM_MASK3 ;
}

void ADC_0_SS0_Handler(void)
{

////    if(ADC0_SSFIFO0_R < set_Curr_B_ADC)
////    {
////        // set
////        GPIO_PORTF_DATA_R ^= 0x04;
////        GPIO_PORTB_DATA_R |= (0xC0 & half_step_seq[sequence]) ;
//////        GPIO_PORTB_DATA_R |= (0xC0 & half_step_seq[sequence]) ;
//////        GPIO_PORTB_DATA_R &= ((~0xC0) | half_step_seq[sequence]) ;
////
////    }
//    if(ADC0_ISC_R & 0x00010000)
//    {
//        GPIO_PORTF_DATA_R ^= 0x04;
//
//        if ( GPIO_PORTB_DATA_R & 0x80 )
//        {
//            GPIO_PORTB_DATA_R &= ~0x80 ;
//        }
//        else if ( GPIO_PORTB_DATA_R & 0x40 )
//        {
//            GPIO_PORTB_DATA_R &= ~ 0x40 ;
//        }
////        GPIO_PORTB_DATA_R |= (0xC0 & half_step_seq[sequence]) ;
////        GPIO_PORTB_DATA_R &= ~ (0xC0 & half_step_seq[sequence]) ;
//
////        GPIO_PORTF_DATA_R ^= 0x04;
//        // Reset upper 2 bits for phase B
////        GPIO_PORTB_DATA_R ^= 0x80 ;
////        GPIO_PORTB_DATA_R &= ~ 0x80 ;
////        GPIO_PORTB_DATA_R |=  0x40 ;
//    }
//    else
//    {
//        GPIO_PORTB_DATA_R |= (0xC0 & half_step_seq[sequence]) ;
//    }


    ADC0_ISC_R |= (ADC_IM_MASK0 | ADC_IM_DCONSS0) ;
    ADC0_DCISC_R |= ADC_DCISC_DCINT0 ;

}

/* Phase A */
/* ADC0 SS1 (PE2/AIN1) with PWM 0 as trigger */
void ADC_0_SS1_Handler(void)
{
    if(ADC0_ISC_R & 0x00020000)
    {
//        GPIO_PORTF_DATA_R ^= 0x04;
//        GPIO_PORTB_DATA_R &= ~ 0x30 ;

        if ( GPIO_PORTB_DATA_R & 0x20 )
        {
            GPIO_PORTB_DATA_R &= ~ 0x20 ;
        }
        else if ( GPIO_PORTB_DATA_R & 0x10 )
        {
            GPIO_PORTB_DATA_R &= ~ 0x10 ;
        }
    }
    else
    {
//        GPIO_PORTB_DATA_R |= (0x30 & half_step_seq[sequence]) ;
//        GPIO_PORTB_DATA_R &= ((~0x30) | half_step_seq[sequence]) ;
    }

    ADC0_ISC_R |= (ADC_IM_MASK1 | ADC_IM_DCONSS1) ;
    ADC0_DCISC_R |= ADC_DCISC_DCINT1 ;
}

/* Phase B */
/* ADC1 SS1 (PE1/AIN2) with PWM 0 as trigger */
void ADC_1_SS1_Handler()
{
//    GPIO_PORTF_DATA_R ^= 0x08;

    if(ADC1_ISC_R & 0x00020000)
    {
//        GPIO_PORTF_DATA_R ^= 0x08;
//        GPIO_PORTB_DATA_R &= ~ 0xC0 ;

        if ( GPIO_PORTB_DATA_R & 0x80 )
        {
            GPIO_PORTB_DATA_R &= ~0x80 ;
        }
        else if ( GPIO_PORTB_DATA_R & 0x40 )
        {
            GPIO_PORTB_DATA_R &= ~ 0x40 ;
        }
    }
    else
    {
//        GPIO_PORTB_DATA_R |= (0xC0 & half_step_seq[sequence]) ;
//        GPIO_PORTB_DATA_R &= ((~0xC0) | half_step_seq[sequence]) ;
    }
    ADC1_ISC_R |= (ADC_IM_MASK1 | ADC_IM_DCONSS1) ;
    ADC1_DCISC_R |= ADC_DCISC_DCINT1 ;
}
