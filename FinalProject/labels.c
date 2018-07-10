/************************
 * labels.c
 *        
 *   Authors: Nick Palumbo         
 *   Date Created: 10 Apr. 2016     
 *   Date Last Modified: 14 Apr. 2016    
 *   Assignment: FinalProject           
 *   Part of: Final Project          
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers#include <f3d_uart.h>
#include <f3d_lcd_sd.h>      // Pull in include file for the local drivers
#include <f3d_i2c.h>
#include <f3d_nunchuk.h>
#include <f3d_systick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "easyHelpers.h"
#include "labels.h"
#include "selectScreen.h"

extern nunchuk_t nunchukData; // used to read nunchuck data
extern int winner;
extern char *wavFiles[]; // main.c 

// draw the starting screen when code first runs
void displayStartScreen(void) {
  f3d_lcd_fillScreen(BLUE);

  f3d_lcd_drawString(10,10,"Welcome to REDRAW",WHITE,BLUE);
  delay(100);
  drawCell(16,40,WHITE);
  delay(200);
  drawCell(80,80,WHITE);
  delay(200);
  drawCell(32,40,WHITE);
  delay(200);
  drawCell(16,40,BLUE);
  delay(200);
  drawCell(64,40,WHITE);
  delay(200);
  drawCell(48,40,WHITE);
  delay(200);
  drawCell(64,40,BLUE);

  f3d_lcd_drawString(30,130,"Press C to",WHITE,BLUE);
  f3d_lcd_drawString(40,140,"Continue",WHITE,BLUE);
  
 int pressed = 0;
  while(pressed == 0) {
    f3d_nunchuk_read(&nunchukData);
    while(nunchukData.c && pressed == 0) { // wait till users lets go
      f3d_nunchuk_read(&nunchukData);
      if(!nunchukData.c) {
	pressed = 1;
	break;
      }
    }
    if(pressed == 1) break; 
  }
}

/*
// draw the box for the beginning continue screen
void drawContinueBox(void) {
  int ix,iy;
  for(iy=0;iy<25;iy++){
    for(ix = 32;ix<96;ix++){
      f3d_lcd_drawPixel(ix,iy+135,BLUE);
    }
  }
}
*/

// draw the box for the beginning continue screen
void drawContinueButton(uint16_t backgroundColor, uint16_t textColor) {
  drawBox(30,60,60,backgroundColor);
  f3d_lcd_drawString(37,65,"CONTINUE",textColor,backgroundColor);
}

// draw the box for the beginning continue screen
void drawHomeButton(uint16_t backgroundColor, uint16_t textColor) {
  drawBox(30,80,60,backgroundColor);
  f3d_lcd_drawString(48,85,"HOME",textColor,backgroundColor);
}

void redrawLevelEndSelectionBox(int current, int previous, uint16_t bg, uint16_t tc) {
  // redraw the previous selection blue
  switch(previous){
  case 0: drawBox(30,60,60,bg); f3d_lcd_drawString(37,65,"CONTINUE",tc,bg);break;
  case 1: drawBox(30,80,60,bg); f3d_lcd_drawString(48,85,"HOME",tc,bg);    break;
  }
  
  // redraw the current selection blue
  switch(current){
  case 0: drawBox(30,60,60,tc); f3d_lcd_drawString(37,65,"CONTINUE",bg,tc);break;
  case 1: drawBox(30,80,60,tc); f3d_lcd_drawString(48,85,"HOME",bg,tc);    break;
  }
}

// draw a Message Box // called after user submits design
void drawMessageBox(int winOrLose) {
  int currentSelection = 0, previousSelection = 0, selection = 0;
  uint16_t bg, tc; 
  if(winOrLose) {
      drawBox(20,40,80,BLUE);
      f3d_lcd_drawString(36,45,"SCORE!",WHITE,BLUE);
      currentLevel = (currentLevel % 3) + 1;
      bg = BLUE;
      tc = WHITE;
      // allow user to have option to continue
      drawContinueButton(WHITE,BLUE); // move to next level
      drawHomeButton(BLUE,WHITE); // go back to level select screen
      //playFile(wavFiles[0]);
    } else {
      drawBox(20,40,80,RED);
      f3d_lcd_drawString(36,45,"WRONG!",WHITE,RED);
      bg = RED;
      tc = WHITE;
      // allow user to have option to continue
      drawContinueButton(WHITE,RED); // draw continue
      drawHomeButton(RED,WHITE); // draw exit button
      //playFile(wavFiles[1]);
    }
     
   while(selection == 0){
     f3d_nunchuk_read(&nunchukData);
     
     if(currentSelection != previousSelection) {
       redrawLevelEndSelectionBox(currentSelection,previousSelection,bg,tc);
       previousSelection = currentSelection;
     } 
     
     if(nunchukData.jy > 220) {  //// read up movements //// jy  
       currentSelection--;
       if(currentSelection < 0) currentSelection = 1;
       while(1) { // wait till users lets go                                              
	 f3d_nunchuk_read(&nunchukData);
	 if(!(nunchukData.jy > 220)) break;
       } 
     } 
     
     if(nunchukData.jy < 30) { //// read down movements //// jy  
       currentSelection = (currentSelection + 1) % 2;
       while(1) { // wait till users lets go                                              
	 f3d_nunchuk_read(&nunchukData);
	 if(!(nunchukData.jy < 30)) break;
       }
     }   
     
     if(nunchukData.c) { ////    read c button    //// 
       selection = 1;
       while(nunchukData.c){ // wait till users lets go   
	 f3d_nunchuk_read(&nunchukData);
       }
     }
   }
   // determine to continue with next level or home screen
   switch(currentSelection) {
   case 0: break;
   case 1: gameLoop(); break;
   }
}

void welcomeMessage(void) {
  delay(100);
  printf("**************************************************\n");
  printf("**                                              **\n");
  printf("**               WELCOME TO REDRAW              **\n");
  printf("**                                              **\n");
  printf("**************************************************\n");
  printf("**                                              **\n");
  printf("**  1. A picture will appear on the screen.     **\n");
  printf("**                                              **\n");
  printf("**  2. The picture will be removed once the     **\n");
  printf("**     LED visual timer has 0 lights on.        **\n");
  printf("**                                              **\n");
  printf("**  3. The user will have to redraw the image   **\n");
  printf("**     that was shown during the countdown.     **\n");
  printf("**                                              **\n");
  printf("**  4. If the drawing matches the test then     **\n");
  printf("**     the user moves to the next level.        **\n");
  printf("**                                              **\n");
  printf("**************************************************\n");
  printf("**                  CONTROLS                    **\n");
  printf("**                                              **\n");
  printf("**     C: Paint                                 **\n");
  printf("**     Z: Unpaint                               **\n");
  printf("**     User Button: Submit Picture              **\n");
  printf("**                                              **\n");
  printf("**************************************************\n");
}
