/************************
 * audioPlayerHelpers.h
 *
 *   Authors: Tim Carlson, Nick Palumbo
 *   Date Created: 08 Apr. 2016
 *   Date Last Modified: 08 Apr. 2016
 *   Assignment: Lab 12
 *   Part of: Lab12
 */

#define TIMER 20000
#define AUDIOBUFSIZE 128

int current;

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

extern FATFS Fatfs;		/* File system object */
extern FIL fid;		/* File object */
extern BYTE Buff[512];		/* File read buffer */
extern int ret;


struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

int playFile(char* name);
int outputWavInfo(struct fmtck fck);
void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID);
void drawSelection(int selection, char *wavFiles[]);
void drawLine(int x, int y, uint16_t color);
void drawBar(int selection);
void drawHeader(void);
