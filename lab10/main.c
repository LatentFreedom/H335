/********************************************************** 
 * main.c
 * 
 * Author: Nick Palumbo, Kyle Nealy
 *   Date Created: March 29, 2016
 *   Last Modified by: Kyle Nealy
 *   Date Last Modified: April 1, 2016
 *   Assignment: lab10
 *   Part of: lab10
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <diskio.h>
#include <ff.h>
#include "bmp.h"
#include <stdio.h>

/* Initialize global variables for file access */
FATFS Fatfs;/* File system object */
FIL Fil;/* File object */
FRESULT rc;/* Result codes */
DIR dir;/* Directory object */
FILINFO fno;/* File information object */
UINT bw, br;
BYTE Buff[3];/* File read buffer */
BYTE Headr[54];         /* Used to move past Header */
char *images[] = {"A9.BMP", "POKE.BMP", "BIRD.BMP"};
bmppixel color[128];
struct bmpfile_magic magic;
struct bmpfile_header header;
BITMAPINFOHEADER info;

// check for card read errors
void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

// turns bmppixels into 16-bit (5-6-5) rgb ints 
uint16_t pixelate(bmppixel rgb) {
  uint16_t ret=0x00;  
  uint16_t r_tmp = (rgb.r>>3)<<11;
  uint16_t g_tmp = (rgb.g>>2)<<5;
  uint16_t b_tmp = (rgb.b>>3);
  ret = r_tmp | g_tmp | b_tmp;
  return ret;
}

/* Helper function to draw images to the screen */
void draw(int ind, int state) {
  int row;
  int i;
    
  // Open File and read past .bmp header
  rc = f_open(&Fil, images[ind], FA_READ);
  if (rc) die(rc);
  rc = f_read(&Fil, Headr, sizeof Headr, &br);
  if (rc) die(rc);
  rc = f_read(&Fil, &magic, sizeof(&magic), &br);
  if (rc) die(rc);
  rc = f_read(&Fil, &header, sizeof(header), &br);
  if (rc) die(rc);
  rc = f_read(&Fil, &info, sizeof(info), &br);
  if (rc) die(rc);

  for (row = 0; row < 160; row++){
    uint16_t colorArray[128]; 
    if (state==0) {// portrait
      f3d_lcd_setAddrWindow(0,row,ST7735_width-1,row,0b110);
    } else if (state==1) { // rotate 90
      f3d_lcd_setAddrWindow(0,row,ST7735_width-1,row,0b010);
    } else if (state==2) {// rotate 180
      f3d_lcd_setAddrWindow(0,row,ST7735_width-1,row,0b101);
    } else if (state==3) { // landscape
      f3d_lcd_setAddrWindow(0,row,ST7735_width-1,row,0b011);
    } else {}
    
    // get colors for evry bmppix in row 
    // and populate colorArray w/ (5-6-5) rgb vals 
    rc = f_read(&Fil,&color,sizeof(color),&br);
    for (i=0; i<sizeof(color)/sizeof(color[i]); i++) {
      colorArray[(i+18)%128] = pixelate(color[i]);
    }    
    // push each row of rgb colors to the lcd 
    f3d_lcd_pushColor(colorArray,128);
  }

  // Close the file
  if (rc) die(rc);
  rc = f_close(&Fil);
  if (rc) die(rc);
}

int main(void) {
  
  // I/O Buffing
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  // Initialize Devices
  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  f_mount(0, &Fatfs); // Register volume work area (never fails) 
  
  // Declare variables
  float accel[3];
  nunchuk_t nun;
  int ori = 0;
  int img = 0;
  int j = 0;
  int l_nu = 1;
  int r_nu = 1;
  int c_bt = 1;
  int z_bt = 1;
  
  //clear screen, draw first image
  f3d_lcd_fillScreen(WHITE);
  draw(img,ori);

  while(1) {
    // Read from nunchuk
    f3d_nunchuk_read(&nun);
    int x = (int) nun.jx;
    int c = (int) nun.c;
    int z = (int) nun.z;
    /* The following if statements handle nunchuk input to change images */
    if (x <= 30) { 
      if (l_nu == 1) {
	img-=1;
	img=img%3;
	if (img<0) img=2;
	draw(img, ori);
	l_nu=0;
      }
    } else {l_nu=1;}

    if (x >= 200) {
      if (r_nu==1) {
	img+=1;
	img=img%3;
	draw(img, ori);
	r_nu=0;
      }
    } else {r_nu=1;}

    if (c==1) {
      if (c_bt==1) {
	img+=1; 
	img=img%3;
	draw(img, ori);
	c_bt=0;
      }
    } else { c_bt=1;}
    
    if (z==1) {
      if (z_bt==1) {
	img-=1; 
	img=img % 3;
	if (img<0) img=2;
	draw(img, ori);
	z_bt=0;
      }
    } else { z_bt=1;}

    /* Read from accelerometer to change orientation */
    f3d_accel_read(accel);
    
    
    if (accel[0] > .9 && ori != 0) {
      ori=0;
      f3d_lcd_fillScreen(WHITE);
      draw(img,ori);
    } else if (accel[0] < -.9 && ori != 1) {
      ori=1;
      f3d_lcd_fillScreen(WHITE);
      draw(img,ori);
    } else if (accel[1] < -.9 && ori != 2) {
      ori=2;
      f3d_lcd_fillScreen(WHITE);
      draw(img,ori);
    } else if (accel[1] > .9 && ori != 3) {
      ori=3;
      f3d_lcd_fillScreen(WHITE);
      draw(img,ori);
    } else {}
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
