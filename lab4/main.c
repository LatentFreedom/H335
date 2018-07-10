/********************************************************** 
* main.c
* These definitions allow interction with leds on the STM32 
* Author: Nick Palumbo, Kellen Adolf
*   Date Created: February 5, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: February 12, 2016
*   Assignment: lab4
*   Part of: lab4, using LED lights on the STM32
*/

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  
  while (1) {
    int i;
    for(i = 0;i < 8;i++){
      while(user_btn_read()){
	f3d_led_on(i);
      }
      f3d_led_on(i);
      delay();
      f3d_led_off(i);
    }
    while(user_btn_read()){
      f3d_led_all_on();
    }
    f3d_led_all_on();
    delay();
    f3d_led_all_off();
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
