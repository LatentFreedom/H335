/********************************************************** 
* main.c
* This is the main file that gives output on the LCD screen given GYRO data
* Author: Nick Palumbo, Ethan Lawton
*   Date Created: February 28, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: March 2, 2016
*   Assignment: lab7
*   Part of: lab7
*/

#include <stm32f30x.h>      // Pull in include files for F30x standard drivers
#include <f3d_led.h>        // Pull in include file for the local drivers
#include <f3d_user_btn.h>   // includes functions for user button
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <f3d_delay.h>        // includes delay function to be used
#include <string.h>           // used for sprintf
#include "lcd_app_helpers.h"  // includes helper functions

#define TIMER 20000

int main(void) {

  // Initialize stm32 board utilities
  f3d_led_init();
  f3d_uart_init();
  f3d_user_btn_init();
  f3d_gyro_init();
  f3d_lcd_init();

  // used for saleae logic analyzer screen fill tests
  //f3d_lcd_fillScreen(RED);
  //f3d_lcd_fillScreen2(RED);

  // baseline: the line that is in the middle to mark the changes from positive to negative
  // axis    : X | Y | Z
  int baseline = 75, axis = 0, height;

  // float array to hold data collected from the GYRO
  float data[3];

  // characters to hold the strings of the data collected for the given axis
  char x[0], y[0], z[0];

  // bar color     : the color of the axis bar
  // marker color  : the color for the line that shows GYRO data in visual form
  // baseline color: the color that the baseline is
  uint16_t barColor, markerColor, baselineColor;
  
  // clear the screen
  clearScreen(WHITE);

  // draw X label because starting on X-axis
  f3d_lcd_drawChar(1,1,'X',BLACK,WHITE);
  drawBars(RED);

  // show X in LEDs because starting on X-axis
  makeXLED();
  
  while(1) {
   
    //Read the gyro data
    f3d_gyro_getdata(data);

    // get correct colors given axis
    barColor = correctBarColor(axis);
    markerColor = correctLineColor(axis);
    baselineColor = correctBaselineColor(axis);

    // draw x-axis velocity line
    height = ((int)data[0] * -1) + 75;
    correctHeight(height,10,0,barColor,markerColor,baselineColor);

     // draw y-axis bar
    height = ((int)data[1] * -1) + 75;
    correctHeight(height,50,0,barColor,markerColor,baselineColor);

    // draw z-axis bar
    height = ((int)data[2] * -1) + 75;
    correctHeight(height,90,0,barColor,markerColor,baselineColor);
    
    // given an axis number, output the correct axis label (X | Y | Z) and output the desired data to LCD
    switch(axis){
    case 0:
      f3d_lcd_drawChar(1,1,'X',BLACK,WHITE);
      sprintf(x,"%f",data[0]);  // convert float into a string
      f3d_lcd_drawString(20,1,x,BLACK,WHITE);  //Print the gyro data
      break;
    case 1:
      f3d_lcd_drawChar(1,1,'Y',WHITE,BLACK);
      sprintf(y,"%f",data[1]);
      f3d_lcd_drawString(20,1,y,WHITE,BLACK);
      break;
    case 2:
      f3d_lcd_drawChar(1,1,'Z',WHITE,BLUE);
      sprintf(z,"%f",data[2]);
      f3d_lcd_drawString(20,1,z,WHITE,BLUE);
      break;
    }
    
    // check for user button press
    if(user_btn_read()){
      axis = readButton(axis); // increment axis and output correct bars and background colors
      while(user_btn_read());  // while user button is held, stop processing so there are no exta increments
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
