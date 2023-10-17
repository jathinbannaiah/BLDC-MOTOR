/*
 * utils.h
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#include <string.h>

#endif /* UTILS_H_ */

void strclr(char *str_clr);

int compare_str( char *str1, char *str2);

int compare_int( int *arr1, int *arr2);

void int_to_string(int num, char *str);

int string_to_int(char *str);

float map(float x, float in_min, float in_max, float out_min, float out_max);

void delayms(int n);

void delayOneMs();

void delaydebounce( int n );

void delayus(int n);
