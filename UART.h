/*
 * UART.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef UART_H_
#define UART_H_





#endif /* UART_H_ */

#ifndef stdint_H_
#define stdint_H_ /*header definition starts here*/
#include"stdint.h"
#endif /* header file ends here*/

#ifndef string_h
#define string_h /*header definition starts here*/
#include"string.h"
#endif /* header file ends here*/

#ifndef tm4c123gh6pm_H
#define tm4c123gh6pm_H /*header definition starts here*/
#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"
#endif /* header file ends here*/

#ifndef interrupt_h
#define interrupt_h /*header definition starts here*/
#include"interrupt.h"
#endif /* header file ends here*/

#ifndef utils
#define utils /*header definition starts here*/
#include"utils.h"
#endif /* header file ends here*/

void uart0_init(void);

char uart0_read(void);

void uart0_send_char(char TX_DATA);

void uart0_send_str(char* TX_DATA_STR);

void uart0_send_int(int number);

void uart1_init(void);

char uart1_read(void);

void uart1_send_char(char TX_DATA);

void uart1_send_str(char* TX_DATA_STR);

void uart1_send_int(int number);
