/********************************************************** 
* main.c
* This main is part of testing printf as a debugging tool.
* Author: Nick Palumbo
*   Date Created: February 26, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: February 26, 2016
*   Assignment: lab7
*   Part of: lab7
*/

//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <stm32f30x.h>

// Simple looping delay function
void delay(void) {
  int i = 500000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

// turn LED on and off
void strobeLED(void) {
  f3d_led_on(0);
  f3d_led_off(0);
}

// turn LED on and off with a printf called multiple times between
void strobeLEDwithPrintf(void) {
  int i = 0;
  f3d_led_on(0);
  for(i = 0;i < 100;i++)
    printf("%d",i);
  f3d_led_off(0);
 for(i = 0;i < 100;i++)
    printf("%d",i);
}

int main(void){

  f3d_uart_init();
  f3d_led_init();
  f3d_user_btn_init();

  // Allows use of printf to buffer
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);


  int i = 0;
  while(1)
    {
      // read user button and increment if button is pressed
      if(user_btn_read()) 
      {
        i++;
        // if user button is held then do not increment more and wait
        while(user_btn_read());
      }
      
      // 
      if(i%2 == 0)
      {
        strobeLED();
      }
      else 
      {  
        strobeLEDwithPrintf();
      }



    }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
