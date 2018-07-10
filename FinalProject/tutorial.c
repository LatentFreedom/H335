/************************
 * tutorial.c
 *        
 *   Authors: Nick Palumbo         
 *   Date Created: 10 Apr. 2016     
 *   Date Last Modified: 14 Apr. 2016    
 *   Assignment: FinalProject           
 *   Part of: Final Project          
 */

#include <stdio.h>
#include <f3d_lcd_sd.h>
#include <f3d_led.h>
#include <f3d_nunchuk.h>
#include "tutorial.h"

extern int currentCell; // current cell selected
extern int lastCell;  // last cell selected

extern int cellsOn[32];

extern int x;
extern int y;

extern nunchuk_t nunchukData; // used to read nunchuck data
extern int ledMiliTimer;

extern test1[]; // tests.h



// walk user through a tutorial for redraw
void tutorial(void) {
  int i,ix,iy,selection = 0;
  f3d_lcd_fillScreen(BLACK); 
  f3d_led_all_on();
  
  f3d_lcd_drawString(0,50,"1. Picture appears.",WHITE,BLACK);
  delay(1000);
  drawTest1(); // tests.c 
  delay(1000);
  f3d_lcd_drawString(0,70,"2. The picture will",WHITE,BLACK);
  f3d_lcd_drawString(20,80,"disappear when",WHITE,BLACK);
  f3d_lcd_drawString(20,90,"the LEDs are all",WHITE,BLACK);
  f3d_lcd_drawString(20,100,"off.",WHITE,BLACK);
  
  // loop through LEDs and turn them off
  for(i = 7;i>=0;i--) {
    delay(ledMiliTimer); 
    f3d_led_off(i);
  }

  // draw Continue Box
  for(iy=0;iy<12;iy++){
    for(ix = 35;ix<95;ix++){
      f3d_lcd_drawPixel(ix,iy+138,WHITE);
    }
  }
  f3d_lcd_drawString(40,140,"CONTINUE.",BLUE,WHITE);

  while(selection == 0) {
    f3d_nunchuk_read(&nunchukData);
    if(nunchukData.c == 1) selection = 1;
  }
  while(nunchukData.c == 1) { // wait till users lets go                                              
    f3d_nunchuk_read(&nunchukData);
  }
  tutorialStep2();
}

void labels(void) {

  f3d_lcd_drawString(0,50,"3. Redraw the image",WHITE,BLACK);
  f3d_lcd_drawString(0,60,"   that was shown.",WHITE,BLACK);

  f3d_lcd_drawString(0, 80,"C: Paint",WHITE,BLACK);
  f3d_lcd_drawString(0, 90,"Z: Unpaint",WHITE,BLACK);
  f3d_lcd_drawString(0,100,"User-Button: Submit",WHITE,BLACK);
}


void tutorialStep2(void) {
  f3d_led_all_off();
  f3d_lcd_fillScreen(BLACK);
  labels();
  tutorialGame();
  while(1);
}


void tutorialGame(void) {

  int xi,yi;
  x = 0;
  y = 0;
  currentCell = 0;
  lastCell = 0;
  drawSelector(x,y);
  while(1){
    //labels();
    f3d_nunchuk_read(&nunchukData);
    readNunchukData();      // finalAppHelpers.c
    if(!(currentCell == lastCell)) {
	drawSelector(x,y);      // finalAppHelpers.c
      lastCell = currentCell;
    }
    readUserButtonTutorial();
  }
}

void readUserButtonTutorial(void) {                                                    
  if(user_btn_read()) {
    // determine if user is winner
    checkCorrectnessTutorial(0); // tests.c
  }
}

void checkCorrectnessTutorial(int currentLevel) {
  int i,ix,iy;
  int count = 0, selection = 0;
  int levelCellsOn;
  // determine the number of on cells given current level
  levelCellsOn = 4;

  int on = 0; 
  for(i = 0;i<32;i++) {
    if(cellsOn[i]){
      on++;
      if(i == test1[count]) count++;
    }
  }
  if(count == levelCellsOn && count == on) {
    f3d_lcd_fillScreen(BLACK);
    f3d_lcd_drawString(10,50,"Now you know how",WHITE,BLACK);
    f3d_lcd_drawString(10,60,"to play REDRAW!",WHITE,BLACK);
  } else {
    f3d_lcd_fillScreen(BLACK);
    f3d_lcd_drawString(10,20,"You didn't draw",WHITE,BLACK);
    f3d_lcd_drawString(10,30,"it exactly right..",WHITE,BLACK);
    f3d_lcd_drawString(10,40,"However...,",WHITE,BLACK);
    f3d_lcd_drawString(10,50,"Now you know how",WHITE,BLACK);
    f3d_lcd_drawString(10,60,"to play REDRAW!",WHITE,BLACK);
  }
  // draw Continue Box
  for(iy=0;iy<12;iy++){
    for(ix = 35;ix<95;ix++){
      f3d_lcd_drawPixel(ix,iy+138,WHITE);
    }
  }
  f3d_lcd_drawString(40,140,"CONTINUE.",BLUE,WHITE);
  
  // wait for user input
  while(selection == 0) {
    f3d_nunchuk_read(&nunchukData);
    if(nunchukData.c == 1) selection = 1;
  }
  while(nunchukData.c == 1) { // wait till users lets go                                              
    f3d_nunchuk_read(&nunchukData);
  }
  x = 0; // reset x coordinate to top left      
  y = 0; // reset y coordinate to top left
  drawSelector(x,y); // redraw the selector       
  currentCell = 0; // set current cell to top left 
  lastCell = currentCell; // set last cell to the top left  
  createCellData(); // remove on cell data
  gameLoop(); // redraw.c
}
