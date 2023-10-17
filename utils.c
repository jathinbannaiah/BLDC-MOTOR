/*
 * utils.c
 *
 *  Created on: 15-Apr-2023
 *      Author: Sarvjit
 */

#include"utils.h"

//void strclr(char *str_clr)
//{
//    for(int i = 0 ; i < sizeof(str_clr) ; i ++)
//    {
//        *str_clr = '\0' ;
//        str_clr ++;
//    }
//}

int compare_str( char *str1, char *str2)
{
    int i = 0 ;
    while ( i < strlen(str2) )
    {
        if (str1[i] != str2[i])
        {
            return 0 ;
        }

        i++ ;
    }

    return 1 ;
}

int compare_int( int *arr1, int *arr2)
{
    int i = 0 ;
    while ( i < sizeof(arr1) )
    {
        if (arr1[i] != arr2[i])
        {
            return 0 ;
        }

        i++ ;
    }

    return 1 ;
}

void int_to_string(int num, char *str) {
    int i = 0;
    int sign = 1;

    if (num < 0) {
        sign = -1;
        num = -num;
    }

//    do {
//        str[i++] = num % 10 + '0';
//        num /= 10;
//    } while (num > 0);

    while (num > 0)
    {
            str[i++] = num % 10 + 48;
            num /= 10;
    }

    if (sign < 0) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int j = 0;
    char temp;
    while (j < i/2) {
        temp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = temp;
        j++;
    }
}

int string_to_int(char *str) {
    int num = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        str++;
    }

    for( int i = 0; i < sizeof(str); i ++)
    {
        num = ( num * 10 ) + ( str[i] - 48 ) ;
    }

    return num * sign;
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void delaydebounce( int n )
{
    int i, j ;

    for(i = 0; i < n ; i ++)
    {
        for(j = 0; j < 3180; j++);
    }
}

void delayms( int n )
{
    int i, j ;

    for(i = 0; i < n ; i ++)
    {
        for(j = 0; j < 3180; j++);
    }
}
void delayus( int n )
{
    int i, j ;

    for(i = 0; i < n ; i ++)
    {
        for(j = 0; j < 1; j++);
    }
}
