/************************
 * auido.c
 *        
 *   Authors: Nick Palumbo         
 *   Date Created: 10 Apr. 2016     
 *   Date Last Modified: 14 Apr. 2016    
 *   Assignment: FinalProject           
 *   Part of: Final Project          
 */


#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_uart.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <f3d_dac.h>
#include "audio.h"

FIL fid;
int ret;

#define WAV_COUNT 2
extern char *wavFiles[];// = {"thermo.wav","thermo.wav"};

//
void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

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

int outputWavInfo(struct fmtck fck) {
  
  printf("audio format 0x%x\n", fck.wFormatTag);
  printf("channels %d\n", fck.nChannels);
  printf("sample rate %d\n", fck.nSamplesPerSec);
  printf("data rate %d\n", fck.nAvgBytesPerSec);
  printf("block alignment %d\n", fck.nBlockAlign);
  printf("bits per sample %d\n", fck.wBitsPerSample);
  
  return 0;
}

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

