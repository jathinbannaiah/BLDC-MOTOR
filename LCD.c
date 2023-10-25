/*
 * LCD.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"LCD.h"

#include"utils.h"

void LCD_init(void)
{
    /*
     * RS -> A6
     * R/W -> GND
     * E -> A7
     * Data -> PB0 ~ PB7
     */
//    SYSCTL_RCGC2_R |= 0x0000003;       /* enable clock to GPIOA GPIOB at clock gating control register */
//
//    GPIO_PORTA_AFSEL_R &= ~ 0xC0;      /* 6) disable alt funct on PF7-0 */
//
//    GPIO_PORTB_AFSEL_R &= ~0xFF;
//
//    GPIO_PORTA_AMSEL_R &= ~0xFF;      /* 3) disable analog on PF */
//
//    GPIO_PORTB_AMSEL_R &= ~0xFF;
//
////    GPIO_PORTA_LOCK_R = 0x4C4F434B;     /* unlock commit register */
////
////    GPIO_PORTB_LOCK_R = 0x4C4F434B;     /* unlock commit register */
//
//    GPIO_PORTA_DIR_R |= 0xC0;            /* enable the GPIO pins for the LCD as output */
//
//    GPIO_PORTB_DIR_R |= 0xFF ;
//////
////    GPIO_PORTA_PUR_R = 0x11;
//
//    GPIO_PORTA_DEN_R |= 0xC0;       /* enable the GPIO pins for digital function */
//
//    GPIO_PORTB_DEN_R |= 0xFF;       /* enable the GPIO pins for digital function */
//
//    GPIO_PORTA_DIR_R |= 0xC0;
//
//    GPIO_PORTB_DIR_R |= 0xFF;

//    GPIO_PORTA_DATA_R |= 0x80 ;

    LCD_Send_Command(1);

    delaydebounce(5);

    LCD_Send_Command(0x38);

    delaydebounce(5);

    LCD_Send_Command(0x0C);

    delaydebounce(5);

    LCD_Send_Command(0x80);

    delaydebounce(5);

    LCD_Clear();

}
void LCD_Clear(void)
{
    LCD_Send_Command(1);

//    delaydebounce(2);
}
void LCD_Set_Cursor(int row, int column)
{
    int CMD_here ;
    if (row == 0)
    {
        CMD_here = 0x80 ;
    }
    else if ( row == 1 )
    {
        CMD_here = 0xC0 ;
    }

    CMD_here = CMD_here + column ;

    LCD_Send_Command(CMD_here) ;
//    delaydebounce(5);
}


void LCD_Send_Command(int CMD)
{
    GPIO_PORTA_DATA_R &= ~EN ;
    GPIO_PORTA_DATA_R &= ~ RS ;
    GPIO_PORTB_DATA_R = CMD ;
    GPIO_PORTA_DATA_R |= EN ;
    delayus(10);
    GPIO_PORTA_DATA_R &= ~ EN ;
//    delaydebounce(1);

}

void LCD_Send_Data(int DATA)
{
    GPIO_PORTA_DATA_R &= ~EN ;
    GPIO_PORTA_DATA_R |= RS ;
    GPIO_PORTB_DATA_R = DATA ;
    delayus(10);
    GPIO_PORTA_DATA_R |= EN ;
    delayus(1);
    GPIO_PORTA_DATA_R &= ~ EN ;
    GPIO_PORTA_DATA_R |= RS ;
}

void LCD_Send_String( char* str )
{
    for (int i = 0; i < strlen(str) ; i++)
    {
        LCD_Send_Data(str[i]) ;
//        delaydebounce(1);
    }
}

void LCD_Send_int(int number)
{
    char str[10] = "";
    int_to_string(number, str);
    LCD_Send_String(str) ;
}
