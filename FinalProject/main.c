/************************
 * main.c
 *
 *   Authors: Nick Palumbo
 *   Date Created: 10 Apr. 2016
 *   Date Last Modified: 14 Apr. 2016
 *   Assignment: FinalProject
 *   Part of: Final Project
 */

/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
//#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
//#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
//#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <f3d_dac.h>
#include <f3d_user_btn.h>

#include "selectScreen.h"

#include "tests.h"
#include "hardTests.h"
#include "audio.h"
#include "labels.h"
#include "redraw.h"

#include "hardHelpers.h"
#include "easyHelpers.h"

#include "tutorial.h"

int ledMiliTimer = 100;

FATFS Fatfs;		/* File system object */
char *wavFiles[] = {"canoli.WAV","fredo.wav"};


int currentCell = 0; // current cell selected
int lastCell = 0;  // last cell selected

nunchuk_t nunchukData; // used to read nunchuck data
RTC_TimeTypeDef  RTC_TimeStructure; // used for random time

int currentLevel = 0;
int lastLevel = 0;
  
int cellsOn[32];
int cellsOnHard[64];

int easyArray[32];
int cellArray[64];

int x = 0;
int y = 0;

int mode = 0; // 0: easy | 1: hard

int cellCount = 1; // number of cells to make

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();     delay(10);
  f3d_timer2_init();   delay(10);
  f3d_dac_init();      delay(10);
  f3d_i2c1_init();     delay(10);
  f3d_delay_init();    delay(10);
  f3d_rtc_init();      delay(10);
  f3d_systick_init();  delay(10);
  f3d_lcd_init();      delay(10);
  f3d_nunchuk_init();  delay(10);
  f3d_user_btn_init(); delay(10);
  f3d_led_init();

  welcomeMessage();       // labels.c
  //createCellData();    // finalAppHelpers.c
  createHardCellData(); // hardHelpers.c

  f3d_nunchuk_read(&nunchukData);

  displayStartScreen(); // selectScreens.c
  gameLoop(); // redraw.c
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
