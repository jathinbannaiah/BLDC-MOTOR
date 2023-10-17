/*
 * interrupt.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"interrupt.h"

/*********** DisableInterrupts ***************
*
* disable interrupts
*
* inputs:  none
* outputs: none
*/


void DisableInterrupts(void)
{
    __asm ("    CPSID  I\n");
}

/*********** EnableInterrupts ***************
*
* emable interrupts
*
* inputs:  none
* outputs: none
*/

void EnableInterrupts(void)
{
    __asm  ("    CPSIE  I\n");
}

/*********** WaitForInterrupt ************************
*
* go to low power mode while waiting for the next interrupt
*
* inputs:  none
* outputs: none
*/

void WaitForInterrupt(void)
{
    __asm  ("    WFI\n");
}
