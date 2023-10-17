/*
 * interrupt.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_





#endif /* INTERRUPT_H_ */


#include <stdint.h>

#include "inc/tm4c123gh6pm.h"

void EnableInterrupts(void);

void WaitForInterrupt(void);

void DisableInterrupts(void);
