///*
// * QEI.c
// *
// *  Created on: 19-Aug-2023
// *      Author: B Praveen
// */
//
//#include "QEI.h"
//
//#ifndef UART_H_
//#include "UART.h"
//#endif
//
//void QEI_0_INIT(void)
//{
//    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_QEI0;
//    SYSCTL_RCGCQEI_R |= SYSCTL_RCGCQEI_R0;
//
//    while ((SYSCTL_PRQEI_R & SYSCTL_PRQEI_R0) == 0) {}; // Wait for clock to stabilize
//
//    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC; // Enable clock for GPIO Port C
//    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; // Enable and provide a clock to GPIO Port C in Run mode
//
//    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R2) == 0) {}; // Wait for clock to stabilize
//
//    GPIO_PORTC_DIR_R |= 0x00000000; // Set GPIO Port C Direction as INPUT
//    GPIO_PORTC_DEN_R |= 0xFF; // Enable the GPIO pins for digital function
//
//    GPIO_PORTC_AFSEL_R |= 0xF0; // Enable alternate function on the QEI pins
//    GPIO_PORTC_AMSEL_R &= ~0xFF; // Disable analog function on the QEI pins
//    GPIO_PORTC_PCTL_R |= 0x00003333; // Configure the pins for QEI function
//
//    QEI0_CTL_R |= 0x00000038; // Configure QEI control parameters
//    QEI0_MAXPOS_R = 0x00001FFF; // Set the maximum position value
//    QEI0_LOAD_R = 160000 - 1; // Set the velocity capture reload value
//    QEI0_CTL_R |= 0x00000001; // Enable the QEI
//
//    /* Interrupt Configuration */
//    EnableInterrupts(); // Enable global Interrupt flag (I)
//    NVIC_PRI3_R |= (NVIC_PRI3_R & 0xFFFF1FFF) | 0x0000C000; // Set the priority for QEI interrupt
//    NVIC_EN0_R |= 0x00002000; // Enable interrupt 13 in NVIC
//    QEI0_INTEN_R |= 0x00000005; // Enable specific QEI interrupts
//}
//
//char Q_str[100];
//long rpm = 0;
//
//void QEI_0_Handler(void)
//{
//    if (QEI0_RIS_R & 0x00000004)
//    {
//        QEI0_ISC_R |= 0x00000004;
//        // Handle direction change
//    }
//
//    if (QEI0_RIS_R & 0x00000001)
//    {
//        QEI0_ISC_R |= 0x00000001;
//        // Handle velocity capture
//    }
//}

