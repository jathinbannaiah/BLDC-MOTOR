/*
 * UART.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"UART.h"

#include"interrupt.h"

#include"utils.h"

#define MAX_BUF_SIZE 500

char RX0_DATA = '\0';
char RX1_DATA = '\0';

char RX0_BUF[500], RX1_BUF[500], RX1_CMD[500] ;

int backspace_counter = 0, rx0_command_flag = 0, rx1_command_flag = 0 ;

/* UART0 */

void uart0_init(void)
{
    SYSCTL_RCGC1_R |= 0x00000001 ;

    SYSCTL_RCGCUART_R |= 0x01 ;  //UART Module 0 Run Mode Clock Gating Control
    SYSCTL_RCGCGPIO_R |= 0x01 ;  //GPIO Port A Run Mode Clock Gating Control
    GPIO_PORTA_AFSEL_R |= 0x03 ;
    GPIO_PORTA_PCTL_R |= 0x00000011 ;
    GPIO_PORTA_DEN_R |= 0x03;

    UART0_CTL_R  = 0x00 ;

    /*Baud Rate = 115200*/
    UART0_IBRD_R = 8 ;
    UART0_FBRD_R = 44 ;

    /*Baud Rate = 9600*/
//    UART0_IBRD_R = 104 ;
//    UART0_FBRD_R = 11 ;
    UART0_LCRH_R = 0x60 ;
    UART0_CC_R = 0x00 ;
    UART0_CTL_R  |=  0x0301 ;
    GPIO_PORTA_AMSEL_R &= ~ 0x03 ;

    EnableInterrupts();

    UART0_IM_R |= 0x10;
    UART0_ICR_R |= 0x10;
    NVIC_EN0_R |= 0x20;  // enable UART0 interrupts in the NVIC
    NVIC_PRI1_R |= 0x00006000;

}


char uart0_read(void)
{
    while( (UART0_FR_R & UART_FR_RXFE) );
    char RX0_DATA = UART0_DR_R ;
    return RX0_DATA ;
}

int backspace_flag = 0 ;
void uart0_send_char(char TX_DATA)
{
    while( (UART0_FR_R & UART_FR_TXFF) );
    UART0_DR_R = TX_DATA ;
}

void uart0_send_str(char* TX_DATA_STR)
{
    for (int i = 0; i < strlen(TX_DATA_STR) ; i++)
    {
        uart0_send_char(TX_DATA_STR[i]) ;
    }
}

void uart0_send_int(int number)
{
    char str[10];
    int_to_string(number, str);
    uart0_send_str(str) ;
}

int BUF0_Counter = 0 ;

void Uart0_RX_Handler(void)
{
    rx0_command_flag = 0 ;

    UART0_ICR_R = 0x10;
    RX0_DATA = UART0_DR_R ;

    if(BUF0_Counter < MAX_BUF_SIZE)
    {
        if( ( RX0_DATA == '\b' ) )
        {
            RX0_BUF[BUF0_Counter] = '\0' ;
            if ( !(BUF0_Counter <= 0 ))
            {
                BUF0_Counter -= 1 ;
            }
            else ;
            RX0_BUF[BUF0_Counter] = ' ' ;
//            BUF_Counter += 1 ;
//            uart0_send_char('\n');
//            uart0_send_str(RX0_BUF);
            //RX_BUF[BUF_Counter] = '\0' ;
        }
        else if ( !( BUF0_Counter < 0 ) )
        {
            RX0_BUF[BUF0_Counter] = RX0_DATA ;
            BUF0_Counter += 1 ;
        }
        else ;

    }

    if( ( RX0_DATA == '\r' ) )
    {
        rx0_command_flag = 1 ;
        RX0_BUF[BUF0_Counter] = '\0' ;
        BUF0_Counter = 0 ;
//        uart0_send_str(RX0_BUF);
    }
    uart0_send_char(RX0_DATA);
}

/* UART1 */

void uart1_init(void)
{
    SYSCTL_RCGC1_R |= 0x00000002 ;

    SYSCTL_RCGCUART_R |= 0x02 ;  //UART Module 1 Run Mode Clock Gating Control
    SYSCTL_RCGCGPIO_R |= 0x02 ;  //GPIO Port B Run Mode Clock Gating Control
    GPIO_PORTB_AFSEL_R |= 0x03 ;
    GPIO_PORTB_PCTL_R |= 0x00000011 ;
    GPIO_PORTB_DEN_R |= 0x03;

    UART1_CTL_R  = 0x00 ;

    /*Baud Rate = 115200*/
    UART1_IBRD_R = 8 ;
    UART1_FBRD_R = 44 ;

    /*Baud Rate = 9600*/
//    UART0_IBRD_R = 104 ;
//    UART0_FBRD_R = 11 ;
    UART1_LCRH_R = 0x60 ;
    UART1_CC_R = 0x00 ;
    UART1_CTL_R  |=  0x0301 ;
    GPIO_PORTB_AMSEL_R &= ~ 0x03 ;

    EnableInterrupts();

    UART1_IM_R |= 0x10;
    UART1_ICR_R |= 0x10;
    NVIC_EN0_R |= 0x40;  // enable UART1 interrupts in the NVIC
    NVIC_PRI1_R |= 0x00600000;

}


char uart1_read(void)
{
    while( (UART1_FR_R & UART_FR_RXFE) );
    char RX1_DATA = UART1_DR_R ;
    return RX1_DATA ;
}

//int backspace_flag = 0 ;
void uart1_send_char(char TX_DATA)
{
    while( (UART1_FR_R & UART_FR_TXFF) );
    UART1_DR_R = TX_DATA ;
}

void uart1_send_str(char* TX_DATA_STR)
{
    for (int i = 0; i < strlen(TX_DATA_STR) ; i++)
    {
        uart1_send_char(TX_DATA_STR[i]) ;
    }
}

void uart1_send_int(int number)
{
    char str[10];
    int_to_string(number, str);
    uart1_send_str(str) ;
}

int BUF1_Counter = 0 ;

void Uart1_RX_Handler(void)
{
    rx1_command_flag = 0 ;

    UART1_ICR_R = 0x10;
    RX1_DATA = UART1_DR_R ;

    if(BUF1_Counter < MAX_BUF_SIZE)
    {
        if( ( RX1_DATA == '\b' ) )
        {
            RX1_BUF[BUF1_Counter] = '\0' ;
            if ( !(BUF1_Counter <= 0 ))
            {
                BUF1_Counter -= 1 ;
            }
            else ;
            RX1_BUF[BUF1_Counter] = ' ' ;
//            BUF_Counter += 1 ;
            uart1_send_char('\n');
            uart1_send_str(RX1_BUF);
            //RX_BUF[BUF_Counter] = '\0' ;
        }
        else if ( !( BUF1_Counter < 0 ) )
        {
            RX1_BUF[BUF1_Counter] = RX1_DATA ;
            BUF1_Counter += 1 ;
        }
        else ;

    }

    if( ( RX1_DATA == '\r' ) )
    {
        for (int i = 0; i < sizeof(RX1_CMD); i ++)
        {
            RX1_CMD[i] = '\0' ;
        }

        rx1_command_flag = 1 ;
        RX1_BUF[BUF1_Counter] = '\0' ;

        for (int i = 0; i < strlen(RX1_BUF); i ++)
        {
            RX1_CMD[i] = RX1_BUF[i] ;
        }

//        uart0_send_int(strlen(RX1_BUF));
        BUF1_Counter = 0 ;
    }

    uart1_send_char(RX1_DATA);
}

