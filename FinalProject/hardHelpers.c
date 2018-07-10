/************************
 * hardHelpers.c
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
#include <f3d_dac.h>
#include <f3d_user_btn.h>
#include "easyHelpers.h"
#include "labels.h"
#include "hardHelpers.h"

extern int currentCell; // current cell selected
extern int lastCell;  // last cell selected
extern nunchuk_t nunchukData; // used to read nunchuck data
extern int currentLevel;
extern int lastLevel;

extern int x;
extern int y;

extern int cellsOnHard[64];

extern int ledMiliTimer;
extern int winner;

extern int cellCount; // number of cells to make
extern int cellArray[64];

// draw a bar when x is chosen                                                          
void drawHardCell(int x,int y, uint16_t color) {
  int height,i;
  for(i=x+1;i<x+15;i++)
    for(height=y+1;height<y+19;height++) //ST7735_height = 160                  
      f3d_lcd_drawPixel(i,height,color);
}

// draw a horizontal line that is the same length as box
void drawHardHorizontalLine(int x, int y, uint16_t color) {
  int i;
  for(i = x;i<x+16;i++)
    f3d_lcd_drawPixel(i,y,color);
}

// draw a vertical line that is the same height as the box
void drawHardVerticalLine(int x, int y, uint16_t color) {
  int i;
  for(i = y;i<y+20;i++)
    f3d_lcd_drawPixel(x,i,color);
}

// draw highlight around current cell                                                 
void drawHardSelector(int x, int y) {
  drawHardHorizontalLine(x,     y,    GREEN);
  drawHardHorizontalLine(x,     y+19, GREEN);
  drawHardVerticalLine(  x,     y,    GREEN);
  drawHardVerticalLine(  x+15,  y,    GREEN);
}

// remove selector once it is moved                                                    
void removeHardSelector(int x, int y) {
  drawHardHorizontalLine(x,     y,    BLACK);
  drawHardHorizontalLine(x,     y+19, BLACK);
  drawHardVerticalLine(  x,     y,    BLACK);
  drawHardVerticalLine(  x+15,  y,    BLACK);
}

// put 0 in all indexes in cellsOn array                                             
void createHardCellData(void) {
  int i;
  for(i = 0;i<64;i++) {
    cellsOnHard[i] = 0;
  }
}

// visual timer until drawing starts 
void ledHardTimer(void) {
  int timer;
  f3d_led_all_on(); // turn all leds on
  for(timer = 7; timer>=0;timer--) {
    delay(ledMiliTimer);  
    f3d_led_off(timer);
  }
}



// draw box for each axis                                                             
void drawHardCells(uint16_t color) {
  int yi,xi;

  for(yi=0;yi<=160;yi+=20){
    for(xi=0;xi<128;xi+=16) {
      drawCell(xi,yi,color);
    }
  }

}

// print the cells that are on 
void printHardOnCells(void) {
  int i;
  char si;

  for(i = 0; i<64; i++) {
    if(cellsOnHard[i]){
      printf("%d ",i);
    }
  }
}


// read movements from nunchuck
// make correct selections
void readHardNunchukData(void) {
  //////////////////////////////                                                       
  //// read right movements //// jx                                                    
  //////////////////////////////                                                       
  if(nunchukData.jx > 220) {
    // don't move past border to next line left                                        
    if(!(currentCell ==  7 ||
	 currentCell == 15 ||
	 currentCell == 23 ||
	 currentCell == 31 ||
	 currentCell == 39 ||
	 currentCell == 47 ||
	 currentCell == 55 ||
	 currentCell == 63)) {
      currentCell++;
      removeHardSelector(x,y);
      x += 16;
    }
    if(currentCell > 63) { // no cells above 63                                        
      currentCell = 63;
    }
    if(x >= 112) {
      x = 112;
    }
    while(1) { // wait till users lets go                                              
      f3d_nunchuk_read(&nunchukData);
      if(!(nunchukData.jx > 220)) break;
    }
  }
  /////////////////////////////                                                        
  //// read left movements //// jx                                                     
  /////////////////////////////                                                        
  if(nunchukData.jx < 30) {
    // don't move past border to last line right                                       
    if(!(currentCell ==  0 ||
	 currentCell ==  8 ||
	 currentCell == 16 ||
	 currentCell == 24 ||
	 currentCell == 32 ||
	 currentCell == 40 ||
	 currentCell == 48 ||
	 currentCell == 56)) {
      currentCell--;
      removeHardSelector(x,y);
      x -= 16;
    }
    if(currentCell < 0) { // no negative cells                                         
      currentCell = 0;
    }
    if(x <= 0) {
      x = 0;
    }
    while(1) { // wait till users lets go                                              
      f3d_nunchuk_read(&nunchukData);
      if(!(nunchukData.jx < 30)) break;
    }
  }
  ///////////////////////////                                                          
  //// read up movements //// jy                                                       
  ///////////////////////////                                                          
  if(nunchukData.jy > 220) {
    if(!((currentCell - 8) < 0)) {
      currentCell -= 8;
      removeHardSelector(x,y);
      y -= 20;
    }
    while(1) { // wait till users lets go                                              
      f3d_nunchuk_read(&nunchukData);
      if(!(nunchukData.jy > 220)) break;
    }
  }
  /////////////////////////////                                                        
  //// read down movements //// jy                                                     
  /////////////////////////////                                                        
  if(nunchukData.jy < 30) {
    if(!((currentCell + 8) > 63)) {
      currentCell += 8;
      removeHardSelector(x,y);
      y += 20;
    }
    while(1) { // wait till users lets go                                              
      f3d_nunchuk_read(&nunchukData);
      if(!(nunchukData.jy < 30)) break;
    }
  }
  /////////////////////////////                                                        
  ////    read c button    ////                                                        
  /////////////////////////////                                                        
  if(nunchukData.c) {
    drawHardCell(x,y,WHITE);
    cellsOnHard[currentCell] = 1;
    while(1) { // wait till users lets go                                              
      f3d_nunchuk_read(&nunchukData);
      if(!nunchukData.c) break;
    }
  }
  /////////////////////////////                                                        
  ////    read z button    ////                                                        
  /////////////////////////////                                                        
  if(nunchukData.z) {
    drawHardCell(x,y,BLACK);
    cellsOnHard[currentCell] = 0;
    while(1) { // wait till users lets go                                              
      f3d_nunchuk_read(&nunchukData);
      if(!nunchukData.z) break;
    }
    //colorSelect++;
    //if(colorSelect > 8) colorSelect = 8;
  }
}

void readHardUserButton(void) {
  ////////////////////////////////                                                     
  ////    read user button    ////                                                     
  ////////////////////////////////                                                     
  if(user_btn_read()) {
    // determine if user is winner
    //checkCorrectness(currentLevel);     // tests.c
    checkHardCorrectness(currentLevel);  // hardTests.c
    drawMessageBox(winner);             // labels.c
    winner = -1;
    delay(1000);
    f3d_lcd_fillScreen(BLACK);
    f3d_led_all_off();
    if(currentLevel!=lastLevel){
      lastLevel = currentLevel;
    }
    makeHardTest(currentLevel);
    x = 0;   // reset x coordinate to top left      
    y = 0;  // reset y coordinate to top left
    drawHardSelector(x,y);       // redraw the selector       
    currentCell = 0;            // set current cell to top left 
    lastCell = currentCell;    // set last cell to the top left  
    createHardCellData();     // remove on cell data
  }
}
