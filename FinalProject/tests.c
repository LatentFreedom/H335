/************************
 * tests.c
 *
 *   Authors: Nick Palumbo
 *   Date Created: 10 Apr. 2016
 *   Date Last Modified: 14 Apr. 2016
 *   Assignment: FinalProject
 *   Part of: Final Project
 */

#include <stm32f30x.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <stdlib.h>

extern int test1[]; // {0,7,24,31} // used for testing
extern int cellsOn[32];

extern int easyPoints[32][2]; 

extern int *easyLevel;  // hold point values for easy 4x4 grid
extern int cellCount;  // number of cells to make

extern int currentLevel;     // int for current level
extern int lastLevel;       // int for last level
extern int winner;         // tell if user is winner or not
extern int easyArray[32]; // main.c 

void drawTest1(void) {
  drawCell(0  ,  0,WHITE);//  0
  drawCell(112,  0,WHITE);//  7
  drawCell(0  ,120,WHITE);// 24
  drawCell(112,120,WHITE);// 31
}

uint32_t rando1(){
  uint32_t compressed_time = 0;
  RTC_DateTypeDef RTC_DateStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  compressed_time |= RTC_TimeStructure.RTC_Seconds/2;
  printf("\ntime: %d\n",compressed_time);
  return compressed_time;

}

int cmpfunc1 (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void drawEasyLevel(void) {
  srand(rando1());
  int i;
  int x = (rand() % 63); // 123 different positions
  if(x % 2 != 0) x -= 1; // make sure x is even!
  int y = x  + 1;
  int cell = x / 2; // which cell is being drawn
  
  // set each cell to 0 
  for(i=0;i<32;i++){
    easyArray[i] = 0;
  }
  for(i=0;i<cellCount;i++){
    printf("x: %d | y: %d\n",x,y);
    printf("[%d][0]: %3d | [%d][1]: %3d | cell#: %2d \n",cell,*(easyLevel+x),cell,*(easyLevel+y),cell);
    drawCell(*(easyLevel+x),*(easyLevel+y),WHITE);
    easyArray[i] = cell; // place drawn cell number in cellArray
    printf("cellArray[%d] = %d\n",i,easyArray[i]);
    x = (rand() % 63);
    if(x % 2 != 0) x -= 1;
    y = x + 1;
    cell = x / 2;
  }
  // sort Array
  qsort(easyArray,cellCount,sizeof(int),cmpfunc1);
}

void makeTest(void) {
   switch(currentLevel) {
     //case 1: drawTest1(); break; // used for testing
   default: drawEasyLevel(); break;
   }
   ledTimer();  // start timer
   f3d_lcd_fillScreen(BLACK); // fill screen to remove expected drawing
}

// check if user put correct cells on screen 
void checkCorrectness(int currentLevel) {
  int i;
  int count = 0;
  int levelCellsOn = cellCount;

  int on = 0;
  for(i = 0;i<32;i++) {
    if(cellsOn[i]){
      on++;
      if(i == easyArray[count]) {;
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
