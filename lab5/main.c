/********************************************************** 
* main.c
* 
* Author: Nick Palumbo, Alice (Qin Niu)
*   Date Created: February 12, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: February 18, 2016
*   Assignment: lab5
*   Part of: lab5
*/

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_uart_init();
  
  // prevent library from buffing I/O
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  

  int line_count = 0, word_count = 0, character_count = 0;
  char c;
  while(1){  

    while ((c = getchar()) != 0x1b) // end on Esc
    {
      character_count++;
      if(c == ' ')
	{
	  word_count++;
	}
      else if(c == '\n')
	{
	  word_count++;
	  line_count++;
	}

    }
  
    printf("%d %d %d\n",line_count, word_count, character_count);
  }
  return 0;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
