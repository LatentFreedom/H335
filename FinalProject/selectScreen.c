/************************
 * selectScreen.c
 *        
 *   Authors: Nick Palumbo         
 *   Date Created: 10 Apr. 2016     
 *   Date Last Modified: 14 Apr. 2016    
 *   Assignment: FinalProject           
 *   Part of: Final Project          
 */


#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "selectScreen.h"
#include "tutorial.h"

extern nunchuk_t nunchukData; // used to read nunchuck data
extern int currentLevel;
extern int lastLevel;
extern int ledMiliTimer;

extern int mode; // 0: easy | 1: hard

// draw a message box for titles and level select 
void drawBox(int x,int y, int length, uint16_t color) {
  int height,i;
  for(i=x+1;i<x+length;i++)
    for(height=y+1;height<y+15;height++) //ST7735_height = 160                  
      f3d_lcd_drawPixel(i,height,color);
}

// redraw the selction box given the current and previous states
void redrawSelectionBox(int current, int previous) { 
  // redraw the previous selection blue
  switch(previous){
  case 0: drawBox(15,35,70,BLUE); f3d_lcd_drawString(20,40,"Tutorial", WHITE,BLUE);break;
  case 1: drawBox(15,55,70,BLUE); f3d_lcd_drawString(20,60,"Easy: 4x8",WHITE,BLUE);break;
  case 2: drawBox(15,75,70,BLUE); f3d_lcd_drawString(20,80,"Hard: 8x8",WHITE,BLUE);break;
  }

  // redraw the current selection blue
  switch(current){
  case 0: drawBox(15,35,70,WHITE); f3d_lcd_drawString(20,40,"Tutorial", BLUE,WHITE);break;
  case 1: drawBox(15,55,70,WHITE); f3d_lcd_drawString(20,60,"Easy: 4x8",BLUE,WHITE);break;
  case 2: drawBox(15,75,70,WHITE); f3d_lcd_drawString(20,80,"Hard: 8x8",BLUE,WHITE);break;
  }
}

// draw the level select screen
void displayLevelSelectScreen(void) {
  int currentSelection = 0, previousSelection = 0, selection = 0;
  nunchukData.c = 0;
  f3d_lcd_fillScreen(BLACK);
  drawBox(5,5,110,BLUE);  f3d_lcd_drawString(10,10,"Select Difficulty",WHITE,BLUE);
  drawBox(15,35,70,WHITE);f3d_lcd_drawString(20,40,"Tutorial",BLUE,WHITE);
  drawBox(15,55,70,BLUE); f3d_lcd_drawString(20,60,"Easy: 4x8",WHITE,BLUE);
  drawBox(15,75,70,BLUE); f3d_lcd_drawString(20,80,"Hard: 8x8",WHITE,BLUE);
  while(selection == 0){
    f3d_nunchuk_read(&nunchukData);

    if(currentSelection != previousSelection) {
      redrawSelectionBox(currentSelection,previousSelection);
      previousSelection = currentSelection;
    } 

    if(nunchukData.jy > 220) {  //// read up movements //// jy  
      currentSelection--;
      if(currentSelection < 0) currentSelection = 2;
      while(1) { // wait till users lets go                                              
	f3d_nunchuk_read(&nunchukData);
	if(!(nunchukData.jy > 220)) break;
      } 
    } 

    if(nunchukData.jy < 30) { //// read down movements //// jy  
      currentSelection = (currentSelection + 1) % 3;
      while(1) { // wait till users lets go                                              
	f3d_nunchuk_read(&nunchukData);
	if(!(nunchukData.jy < 30)) break;
      }
    }   
                                                      
    if(nunchukData.c) { ////    read c button    ////
      while(nunchukData.c){
	f3d_nunchuk_read(&nunchukData);
      }
      // select and set start level 
      switch(currentSelection) {
      case 0: 
	currentLevel = 0; 
	lastLevel = 0; 
	ledMiliTimer = 500; 
	mode=0;  
	selection = 1; 
	tutorial(); // tutorial.c
	break;
      case 1: currentLevel = 1; lastLevel = 1; ledMiliTimer = 200; mode=0;  selection = 1; break;
      case 2: currentLevel = 1; lastLevel = 1; ledMiliTimer = 200; mode=1;  selection = 1; break;
      }
    }
  }
}
