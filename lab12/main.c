
/************************
 * main.c
 *
 *   Authors: Tim Carlson, Nick Palumbo
 *   Date Created: 08 Apr. 2016
 *   Date Last Modified: 08 Apr. 2016
 *   Assignment: Lab 12
 *   Part of: Lab12
 */

/* Code: */

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
#include "queue.h"
#include "audioPlayerHelpers.h"

FATFS Fatfs;		/* File system object */
int current = 40;

#define WAV_COUNT 4
char *wavFiles[] = {"thermo.WAV","chime.WAV","bell.WAV", "trumpet.WAV"};

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  delay(10);
  f3d_timer2_init();
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_delay_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_systick_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  f3d_lcd_fillScreen(WHITE);

  printf("Reset\n");
  

  f_mount(0, &Fatfs);/* Register volume work area */
  nunchuk_t nunchukData; // {0,0,0,0,0,0}
  
  int selection = 0; // incrmeneter for WAV selection
  int prevSelection = 0; // track previoius selection to redraw if needed

  drawHeader();
  drawBar(40);
  f3d_lcd_drawString(10,40, wavFiles[0], WHITE ,BLUE);
  f3d_lcd_drawString(10,50, wavFiles[1], BLACK ,WHITE);
  f3d_lcd_drawString(10,60, wavFiles[2], BLACK ,WHITE);
  f3d_lcd_drawString(10,70, wavFiles[3], BLACK ,WHITE);

  while(1){

    // read nunchuck data
    f3d_nunchuk_read(&nunchukData);

    //selection = makeSelection();
    if(nunchukData.jy < 30) {
      selection = (selection + 1) % WAV_COUNT;
      while(1) { // wait till users lets go
	f3d_nunchuk_read(&nunchukData);
	if(!(nunchukData.jy < 30)) break;
      }
    }
    if(nunchukData.jy > 220) {
      selection--;
      if(selection < 0) { // no negative selections
	selection = WAV_COUNT - 1;
      }
      while(1) { // wait till users lets go
	f3d_nunchuk_read(&nunchukData);
	if(!(nunchukData.jy > 220)) break;
      }
    }

    if(prevSelection != selection){
      drawSelection(selection, wavFiles);
      prevSelection = selection;
    }

    printf("jy: %d\n",nunchukData.jy);

    if(nunchukData.c) {
      while(1) { // wait till users lets go
	f3d_nunchuk_read(&nunchukData);
	if(!(nunchukData.c)) break;
      }
      switch(selection){
      case 0:
	playFile(wavFiles[0]);
	break;
      case 1:
	playFile(wavFiles[1]);
	break;
      case 2:
	playFile(wavFiles[2]);
	break;
      case 3:
	playFile(wavFiles[3]);
	break;
      }
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
