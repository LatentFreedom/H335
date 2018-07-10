/************************
 * f3d_systick.c
 *
 *   Authors: Curtis Bitner, Nick Palumbo
 *   Date Created: 01 Apr. 2016
 *   Date Last Modified: 01 Apr. 2016
 *   Assignment: Lab 11, Part 4
 *   Part of: Lab 4
 *

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_user_btn.h>
#include <f3d_uart.h>

volatile int systick_flag = 0;
int i = 0;

void f3d_systick_init(void) {
  SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}

void SysTick_Handler(void) {

  if(!user_btn_read()){
    SysTick_Config(SystemCoreClock/100);
  }
  else {
    SysTick_Config(SystemCoreClock/10);
  }
  f3d_led_on(i%8);
  f3d_led_off((i+6)%8);
  
  i++;
}

/* f3d_systick.c ends here */
