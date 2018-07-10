/************************
 * hardTests.c
 *
 *   Authors: Nick Palumbo
 *   Date Created: 10 Apr. 2016
 *   Date Last Modified: 14 Apr. 2016
 *   Assignment: FinalProject
 *   Part of: Final Project
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_lcd_sd.h>
#include <stdio.h>
//#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <stdlib.h>

extern int test1Hard[]; // {0,7,24,31}
extern int test2Hard[]; // {1,6,9,14,17,22,25,30}
extern int test3Hard[]; // {0,9,20,30}

extern int cellPoints[64][2];

extern int *level4;
extern int cellCount; // number of cells to make

extern int cellsOnHard[64];

extern int currentLevel;
extern int lastLevel;

extern int winner;

// extern nunchuk_t nunchukData; // used to read nunchuck data
extern int cellArray[64]; // main.c

void drawHardTest1(void) {
  drawHardCell(0  ,  0,WHITE);//  0
  drawHardCell(112,  0,WHITE);//  7
  drawHardCell(0  ,140,WHITE);// 56
  drawHardCell(112,140,WHITE);// 63
}

uint32_t rando(){
  uint32_t compressed_time = 0;
  RTC_DateTypeDef RTC_DateStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  compressed_time |= RTC_TimeStructure.RTC_Seconds/2;
  printf("\ntime: %d\n",compressed_time);
  return compressed_time;

}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void drawLevel4(void) {
  srand(rando());
  int i;
  int x = (rand() % 123); // 123 different positions
  if(x % 2 != 0) x -= 1; // make sure x is even!
  int y = x  + 1;
  int cell = x / 2; // which cell is being drawn
  
  for(i=0;i<64;i++){
    cellArray[i] = 0;
  }
  for(i=0;i<cellCount;i++){
    printf("x: %d | y: %d\n",x,y);
    printf("[%d][0]: %3d | [%d][1]: %3d | cell#: %2d \n",cell,*(level4+x),cell,*(level4+y),cell);
    drawHardCell(*(level4+x),*(level4+y),WHITE);
    cellArray[i] = cell; // place drawn cell number in cellArray
    printf("cellArray[%d] = %d\n",i,cellArray[i]);
    x = (rand() % 123);
    if(x % 2 != 0) x -= 1;
    y = x + 1;
    cell = x / 2;
  }
  // sort Array
  
  printf("\n");
  for(i=0;i<64;i++){
    printf("%d ",cellArray[i]);
  }
  qsort(cellArray,cellCount,sizeof(int),cmpfunc);
 printf("\n");
  for(i=0;i<64;i++){
    printf("%d ",cellArray[i]);
  }
 printf("\n");
}

void makeHardTest(void) {
   switch(currentLevel) {
     //case 1: drawHardTest1(); break; // used for testing
   default: drawLevel4(); break;
   }
   ledHardTimer();  // start timer
   f3d_lcd_fillScreen(BLACK); // fill screen to remove expected drawing
}



// check if user put correct cells on screen 
void checkHardCorrectness(int currentLevel) {
  int i;
  int count = 0;
  int levelCellsOn = cellCount;

  int on = 0;
  for(i = 0;i<64;i++) {
    if(cellsOnHard[i]){
      on++;
      if(i == cellArray[count]) {;
	count++;
      }
    }
  }

  // return 1 if all test cells were drawn
  if(count == levelCellsOn && count == on) {
    winner = 1;
    cellCount++; // increment number of cells to draw
  } else {
    winner = 0;
  }
}

// used for testing //
/*
// check if user put correct cells on screen 
void checkHardCorrectness(int currentLevel) {
  int i;
  int count = 0;
  int levelCellsOn;
  // determine the number of on cells given current level
  switch(currentLevel) {
    //case 1: levelCellsOn = 4; break;
    //case 2: levelCellsOn = 8; break;
    //case 3: levelCellsOn = 4; break;
  default: levelCellsOn = cellCount;
  }

  int on = 0;
  switch(currentLevel) {
  case 1:
    for(i = 0;i<64;i++) {
      if(cellsOnHard[i]){
	on++;
	if(i == test1Hard[count]) {
	  printf("current level: %d | i cell: %d | t cell: %d\n",currentLevel,i,test1Hard[count]);
	  count++;
	}
      }
    }
    break;
    
  case 2:
    for(i = 0;i<64;i++) {
      if(cellsOnHard[i]){
	on++;
	if(i == test2Hard[count]) {
	  printf("current level: %d | i cell: %2d | t cell: %d\n",currentLevel,i,test2Hard[count]);
	  count++;
	}
      }
    }
    break;
  
  case 3:
    for(i = 0;i<64;i++) {
      if(cellsOnHard[i]){
	on++;
	if(i == test3Hard[count]) {
	  printf("current level: %d | i cell: %d | t cell: %d\n",currentLevel,i,test3Hard[count]);
	  count++;
	}
      }
    }
    break;
  }

  // return 1 if all test cells were drawn
  if(count == levelCellsOn && count == on) {
    winner = 1;
    cellCount++; // increment number of cells to draw
  } else {
    winner = 0;
  }
}
*/
