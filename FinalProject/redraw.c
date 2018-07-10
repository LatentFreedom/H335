/************************
 * redraw.c
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
#include "hardHelpers.h"

#include "selectScreen.h"
#include "audio.h"
#include "labels.h"
#include "redraw.h"


extern int currentCell;         // current cell selected
extern int lastCell;           // last cell selected
extern nunchuk_t nunchukData; // used to read nunchuck data
extern int currentLevel;
extern int lastLevel;
  
extern int cellsOn[32];
extern int cellsOnHard[64];

extern int x;
extern int y;

extern int mode; // 0: easy | 1: hard
extern int cellCount; // number of cells to make

void gameLoop(void){
  displayLevelSelectScreen(); // choose mode
  f3d_lcd_fillScreen(BLACK);
  
  f_mount(0, &Fatfs);       // Register volume work area 
  delay(100);
  int xi,yi;
  x = 0;
  y = 0;
  currentCell = 0;
  lastCell = 0;
  cellCount = 1;

  if(mode == 0) { 
    makeTest(currentLevel); // tests.c
    createCellData();      // finalAppHelpers.c
    drawSelector(x,y);    // finalAppHelpers.c
  } else { 
    makeHardTest(currentLevel); // hardTests.c
    createHardCellData();      // hardHelpers.c 
    drawHardSelector(x,y);    // hardHelpers.c
  }

  while(1){
    f3d_nunchuk_read(&nunchukData);

    if(mode == 0) { 
      readNunchukData();      // finalAppHelpers.c
    } else {
      readHardNunchukData();   // hardHelpers.c
    }

    if(!(currentCell == lastCell)) {

      if(mode == 0) { 
      drawSelector(x,y);      // finalAppHelpers.c
      } else {      
	drawHardSelector(x,y);   // hardHelpers.c
      }
      lastCell = currentCell;
      printfDebug();           // used for debugging // finalAppHelpers.c
    }
    if(mode == 0) { 
      readUserButton();        // finalAppHelpers.c
    } else {
      readHardUserButton();     // hardHelpers.c
    } 
  }
}
