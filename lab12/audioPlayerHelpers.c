/************************
 * audioPlayerHelpers.c
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

FIL fid;
int ret;

extern int current;

int playFile(char* name) {

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;

   //print WAV file name in serialT
  printf("\nOpen %s\n", name);

   // open WAV file and read contents into rc
  rc = f_open(&fid, name, FA_READ);
  
  // if there is no rc error...
  if(!rc) {
    struct ckhd hd; // create structure to hold 
    uint32_t  waveid; // unsigned 32 integer to hold waveid
    struct fmtck fck; // create structure to hold ...

    readckhd(&fid, &hd, 'FFIR'); // 

    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;

    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);

    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }

    outputWavInfo(fck);

     // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
  if (rc) die(rc);
  //while(1);
}

int outputWavInfo(struct fmtck fck){
  
  printf("audio format 0x%x\n", fck.wFormatTag);
  printf("channels %d\n", fck.nChannels);
  printf("sample rate %d\n", fck.nSamplesPerSec);
  printf("data rate %d\n", fck.nAvgBytesPerSec);
  printf("block alignment %d\n", fck.nBlockAlign);
  printf("bits per sample %d\n", fck.wBitsPerSample);
  
  return 0;
}


void removeBar(int y) {
  int i;
  y -= 1;
  for(i = 0; i<=10;i++){
    drawLine(5,y,WHITE);
    y++;
  }
}

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void drawSelection(int selection, char *wavFiles[]) {
  switch(selection){
  case 0:
    removeBar(current);
    drawBar(40);
    f3d_lcd_drawString(10,40, wavFiles[0], WHITE ,BLUE);
    f3d_lcd_drawString(10,50, wavFiles[1], BLACK ,WHITE);
    f3d_lcd_drawString(10,60, wavFiles[2], BLACK ,WHITE);
    f3d_lcd_drawString(10,70, wavFiles[3], BLACK ,WHITE);
    current = 40;
    break;
  case 1:
    removeBar(current);
    drawBar(50);
    f3d_lcd_drawString(10,40, wavFiles[0], BLACK ,WHITE);
    f3d_lcd_drawString(10,50, wavFiles[1], WHITE ,BLUE);
    f3d_lcd_drawString(10,60, wavFiles[2], BLACK ,WHITE);
    f3d_lcd_drawString(10,70, wavFiles[3], BLACK ,WHITE);
    current = 50;
    break;
  case 2:
    removeBar(current);
    drawBar(60);
    f3d_lcd_drawString(10,40, wavFiles[0], BLACK ,WHITE);
    f3d_lcd_drawString(10,50, wavFiles[1], BLACK ,WHITE);
    f3d_lcd_drawString(10,60, wavFiles[2], WHITE ,BLUE);
    f3d_lcd_drawString(10,70, wavFiles[3], BLACK ,WHITE);
    current = 60;
    break;
  case 3:
    removeBar(current);
    drawBar(70);
    f3d_lcd_drawString(10,40, wavFiles[0], BLACK ,WHITE);
    f3d_lcd_drawString(10,50, wavFiles[1], BLACK ,WHITE);
    f3d_lcd_drawString(10,60, wavFiles[2], BLACK ,WHITE);
    f3d_lcd_drawString(10,70, wavFiles[3], WHITE ,BLUE);
    current = 70;
    break;
  }
}

// draw a line 10 pixels wide given x,y coordinate and color
void drawLine(int x, int y, uint16_t color) {
  int i;
  for(i=x;i<(x+90);i++){
    f3d_lcd_drawPixel(i,y,color);
  }
}

void drawBar(int y){
  int i;
  y -= 1;
  for(i = 0; i<=10;i++){
    drawLine(5,y,BLUE);
    y++;
  }
}


void drawHeader() {
  f3d_lcd_drawString(20,10, "Nick & Tim's", BLACK, WHITE);
  f3d_lcd_drawString(30,20, "Dope Sounds", BLACK, WHITE);
  drawLine(10,35,BLACK);
  drawLine(10,85,BLACK);
}
