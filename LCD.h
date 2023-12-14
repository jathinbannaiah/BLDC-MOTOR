/*
 * LCD.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef LCD_H_
#define LCD_H_





#endif /* LCD_H_ */


#include <stdint.h>

#include <string.h>

#include "D:/ti/TivaWare_C_Series-2.2.0.295/TivaWare_C_Series-2.2.0.295/inc/tm4c123gh6pm.h"

#define RS 0x00000040

#define EN 0x00000080

void LCD_init(void);

void LCD_Clear(void);

void LCD_Send_Command(int CMD);

void LCD_Send_Data(int DATA);

void LCD_Send_String(char *str) ;

void LCD_Send_int(int number);

void LCD_Set_Cursor(int row, int column) ;
