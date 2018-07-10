/********************************************************** 
* lcd_app_helpers.h
* This file has all the functions implemented in lcd_app_helpers.c
* Author: Nick Palumbo, Ethan Lawton
*   Date Created: February 28, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: March 2, 2016
*   Assignment: lab7
*   Part of: lab7
*/

/* declarations for lcd_app_helpers.c */

void clearScreen(uint16_t color);
void drawBar(int x,uint16_t color);
void drawBars(uint16_t);
void removeLine(int x, int y, uint16_t color);
void drawSpeedLine(int x, int height, uint16_t barColor, uint16_t markerColor, uint16_t baselineColor);
void drawLabels(void);


int readButton(int i);
void correctHeight(int height, int placement, int i, uint16_t barColor, uint16_t markerColor,uint16_t baselineColor);
uint16_t correctBarColor(int axis);
uint16_t correctLineColor(int axis);
