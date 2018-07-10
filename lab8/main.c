/********************************************************** 
 * main.c
 * 
 * Author: Nick Palumbo, Tim
 *   Date Created: March 4, 2016
 *   Last Modified by: Nick Palumbo
 *   Date Last Modified: March 10, 2016
 *   Assignment: lab8
 *   Part of: lab8
 */

/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include<f3d_led.h> 
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <f3d_delay.h>
#include <math.h>
#include <time.h>
#include "f3d_compass_helpers.h" // app helpers for compass
#define TIMER 20000

int main(void) {
  float a[3]; //
  float b[3]; // 
  int axis = 0; // hold current axis

  int direction; // hold the current direction
  int didChange = 0; // hold value to check if direction label changed

  float pitch ,roll,headingX,headingY,heading;
  char numberToString[10];

  int lastDirection; // record last direction
  int currentDirection; // record current direction
  //N  : 0 
  //NE : 1
  //E  : 2
  //SE : 3
  //S  : 4
  //SW : 5
  //W  : 6
  //NW : 7
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  // Set up initializers
  f3d_lcd_init();
  f3d_led_init();
  f3d_uart_init();
  f3d_user_btn_init();
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);

  f3d_lcd_fillScreen(WHITE);

  while(1){

    // compute pitch and roll
    f3d_accel_read(a); // read data from accelerometer
    pitch = atan(a[0]/sqrt(pow(a[1],2)+pow(a[2],2)));
    roll = atan(a[1]/sqrt(pow(a[0],2)+pow(a[2],2)));

    // compute the heading
    f3d_mag_read(b); // read data from magnetometer
    headingX = (b[0]*cos(pitch))+(b[2]*sin(pitch));
    headingY = (b[0]*sin(roll)*sin(pitch))+(b[1]*cos(roll))-(b[2]*sin(roll)*cos(pitch));
    heading = atan2f(headingX,headingY)*(180/M_PI)+180; // tilt compensation

    // ROLL
    f3d_lcd_drawString(1,1,"Roll",BLACK,WHITE);
    sprintf(numberToString, "%f",roll);
    f3d_lcd_drawString(50,1,numberToString,BLACK,WHITE);

    // PITCH
    f3d_lcd_drawString(1,10,"Pitch",BLACK,WHITE);
    sprintf(numberToString, "%f",pitch);
    f3d_lcd_drawString(50,10,numberToString,BLACK,WHITE);

    // HEADING
    f3d_lcd_drawString(1,20,"Heading",BLACK,WHITE);
    sprintf(numberToString, "%f",heading);
    f3d_lcd_drawString(50,20,numberToString,BLACK,WHITE);
    
    led_drawMarker(currentDirection);
    lcd_drawCircle();
    lcd_drawMarble(pitch,roll);
    currentDirection = calculateDirection(heading, didChange);
    
    // check if Direction has changed
    if(currentDirection != lastDirection){
      // remove last label
      lcd_clearDirection();
      lastDirection = currentDirection;
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
