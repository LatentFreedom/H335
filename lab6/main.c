// Filename: main.c
// Part of: C335 Lab 6
// Created by: Austin Moore (aulmoore), Nick Palumbo (npalumbo)
// Created on: 2/19/2016
// Last modified by: Austin Moore, Nick Palumbo
// Last modified on: 2/23/2016

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

int main(void){

  // Initialize stm32 board utilities
  f3d_gyro_init();
  f3d_uart_init();
  f3d_led_init();
  f3d_user_btn_init();

  // Allows use of printf to buffer
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  // Data structures for while loop
  float data[3];
  int axis = 0;
  int i, c;

while(1)
  {

    //Check for user axis input
    switch(c = getchar_nopause())
      {
	//Cases for x-axis
      case 88:
      case 120:
	axis = 0;
	break;
        //Cases for y-axis
      case 89:
      case 121:
	axis = 1;
	break;
	//Cases for z-axis
      case 90:
      case 122:
	axis = 2;
	break;
	//Default is to fall through
      }

    // Change the axis with the user button
    if(user_btn_read())
      {
	axis = (axis + 1) % 3;
	//Wait until user button is released
	while(user_btn_read());
      }
    
    //Read the gyro data
    f3d_gyro_getdata(data);
	
    //Print the gyro data
    printf("X: %+f Y: %+f Z: %+f Axis: %c\n ", data[0], data[1], data[2], axis+88);
	
    //Light up the leds corresponding to the axis
    //For positive numbers, light the left side of leds
    for(i=0; i<data[axis]-5 && i<125; i+=25)
      {
	f3d_led_on(8-(i/25));
      }
    
    //For negative numbers, light the right side
    for(i=0; i>data[axis]+5 && i>-125; i-=25)
      {
	f3d_led_on(-(i/25));
      }

    // Delay so that the lights don't turn off too quickly
    delay();
	
    //Turn off the lights
    for(i=0; i<8; i++)
      {
	f3d_led_off(i);
      }
  }  
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
