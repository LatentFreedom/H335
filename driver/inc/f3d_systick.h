/************************
 *f3d_systick.h
 *
 *   Authors: Curtis Bitner, Nick Palumbo
 *   Date Created: 01 Apr. 2016
 *   Date Last Modified: 01 Apr. 2016
 *   Assignment: Lab 11
 *   Part of: Lab 11
 *

/* Code: */
#include <stm32f30x.h>

#define SYSTICK_INT_SEC 100

void f3d_systick_init(void);
void SysTick_Handler(void);

/* f3d_systick.h ends here */

