/********************************************************** 
* lcd_app_helpers.c
* These functions help with main.c when creating visuals on the lcd
* Author: Nick Palumbo, Ethan Lawton
*   Date Created: February 28, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: March 2, 2016
*   Assignment: lab7
*   Part of: lab7
*/

// X-axis
// * Background color: WHITE
// * Baseline color  : BLACK
// * Marker color    : WHITE
// * Bar color       : RED

// Y-axis
// * Background color: BLACK
// * Baseline color  : RED
// * Marker color    : BLACK
// * Bar color       : CYAN

// Z-axis
// * Background color: BLUE
// * Baseline color  : BLACK
// * Marker color    : BLUE
// * Bar color       : GREEN



#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_user_btn.h>  // includes functions for user button
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h> // includes delay function to be used
#include <string.h> // used for sprintf

// clear screen with given color
void clearScreen(uint16_t color) {
  f3d_lcd_fillScreen2(color);
}

// draw a bar when x is chosen
void drawBar(int x,uint16_t color) {
  int height,i;
  for(i=x;i<(x + 20);i++)
    for(height=10;height<ST7735_height-10;height++) //ST7735_height = 160
      f3d_lcd_drawPixel(i,height,color);
}

// draw bar for each axis
void drawBars(uint16_t color) {
  drawBar(10, color); // x-axis bar
  drawBar(50, color); // y-axis bar
  drawBar(90, color); // z-axis bar
}

// draw the line to show when positive and negative
void drawBaseLine(int x,int y, uint16_t baselineColor) {
  int i;
  for(i=x;i<(x+30);i++){
    f3d_lcd_drawPixel(i,y,baselineColor);
  }
}

// draw a line 10 pixels wide given x,y coordinate and color
void drawLine(int x, int y, uint16_t color) {
  int i;
  for(i=x;i<(x+20);i++){
    f3d_lcd_drawPixel(i,y,color);
  }
}

// remove axis speed line for next line to be shown
void removeLine(int x, int y, uint16_t color) {
  int i;
  for(i=x;i<(x+20);i++){
    f3d_lcd_drawPixel(i,y,color);
  }
}

// draw line to show axis speed
void drawSpeedLine(int x, int height,uint16_t barColor,uint16_t markerColor, uint16_t baselineColor) {
  drawLine(x,height,markerColor);
  delay(50);
  removeLine(x,height,barColor);
  drawBaseLine((x-5),75,baselineColor);
}

void drawLabels(void) {
  f3d_lcd_drawChar(1,1,'X',BLACK,WHITE);
}

// make an X with the LEDs
void makeXLED(void) {
  f3d_led_all_off();
  f3d_led_on(1);
  f3d_led_on(3);
  f3d_led_on(5);
  f3d_led_on(7);
}

// make a Y with the LEDs
void makeYLED(void) {
  f3d_led_all_off();
  f3d_led_on(7);
  f3d_led_on(1);
  f3d_led_on(4);
}

// make my interpretation of a Z with the LEDs
void makeZLED(void) {
  f3d_led_all_off();
  f3d_led_on(7);
  f3d_led_on(0);
  f3d_led_on(1);
  f3d_led_on(5);
  f3d_led_on(4);
  f3d_led_on(3);
}

// read user button and change lcd screen accordingly
int readButton(int i) {
  i = (i + 1) % 3;
  switch(i){
  case 0:
    makeXLED(); // make LEDs form an X
    clearScreen(WHITE);
    drawBars(RED);
    break;
  case 1:
    makeYLED(); // make LEDs form a Y
    clearScreen(BLACK);
    drawBars(CYAN);
    break;
  case 2:
    makeZLED(); // make LEDs form my interpretation of a Z
    clearScreen(BLUE);
    drawBars(GREEN);
    break;
  }
  // return the current axis
  return i;
}

// calculate the height and draw the appropriate line on bar
// placement: where the line starts to be draw for the axis bar
void correctHeight(int height, int placement, int i, uint16_t barColor, uint16_t markerColor, uint16_t baselineColor) {
  if(height > ST7735_height-10)
    drawSpeedLine(placement,150,barColor,markerColor,baselineColor);
  else if(height < 10)
    drawSpeedLine(placement,10,barColor,markerColor,baselineColor);
  else
    drawSpeedLine(placement,height,barColor,markerColor,baselineColor);
}

// when the button is pressed the axis string changes and the colors should too
// corrects the br color given the axis
uint16_t correctBarColor(int axis) {
  if(axis == 0)
    return RED;
  else if(axis == 1)
    return CYAN;
  else 
    return GREEN;
}

// corrects the marker color given the axis
uint16_t correctLineColor(int axis) {
  if(axis == 0)
    return WHITE;
  else if(axis == 1)
    return BLACK;
  else 
    return BLUE;
}

// corrects the baseline color given the axis
uint16_t correctBaselineColor(int axis) {
  if(axis == 0)
    return BLACK;
  else if(axis == 1)
    return RED;
  else 
    return BLACK;
}
