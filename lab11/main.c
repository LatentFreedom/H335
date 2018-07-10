/************************
 *main.c
 *runs loop to light LEDs in circular pattern
 *  based on interupts
 *
 *   Authors: Curtis Bitner, Nick Palumbo
 *   Date Created: 01 Apr. 2016
 *   Date Last Modified: 01 Apr. 2016
 *   Assignment: Lab 11, Part 4
 *   Part of: Lab 4
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_rtc.h>
#include <stdio.h>
#include <stdint.h>
#include <f3d_systick.h>
#include "queue.h"

void sequenceLED(void);

int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  f3d_systick_init();
  f3d_lcd_init();
  
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(5,10,"LAB 11: Interrupts",BLACK,WHITE);
  f3d_lcd_drawString(10,30,"Rate of Interrupt:",BLACK,WHITE);

  while(1) {

    if(!user_btn_read()){
      f3d_lcd_drawString(20,40,"100/s",BLACK,WHITE);
    }
    else {
      f3d_lcd_drawString(20,40," 10/s",BLACK,WHITE);
    }

  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
