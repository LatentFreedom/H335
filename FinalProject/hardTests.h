/************************
 * hardTests.h
 *
 *   Authors: Nick Palumbo
 *   Date Created: 10 Apr. 2016
 *   Date Last Modified: 14 Apr. 2016
 *   Assignment: FinalProject
 *   Part of: Final Project
 */


int cellPoints[64][2] = {{0,  0},{16,  0},{32,  0},{48,  0},{64,  0},{80,  0},{96,  0},{112,  0},
			 {0, 20},{16, 20},{32, 20},{48, 20},{64, 20},{80, 20},{96, 20},{112, 20},
			 {0, 40},{16, 40},{32, 40},{48, 40},{64, 40},{80, 40},{96, 40},{112, 40},
			 {0, 60},{16, 60},{32, 60},{48, 60},{64, 60},{80, 60},{96, 60},{112, 60},
			 {0, 80},{16, 80},{32, 80},{48, 80},{64, 80},{80, 80},{96, 80},{112, 80},
			 {0,100},{16,100},{32,100},{48,100},{64,100},{80,100},{96,100},{112,100},
			 {0,120},{16,120},{32,120},{48,120},{64,120},{80,120},{96,120},{112,120},
			 {0,140},{16,140},{32,140},{48,140},{64,140},{80,140},{96,140},{112,140}};


int test1Hard[] = {0,7,56,63}; // used for testing

int *level4 = *cellPoints;
extern int cellCount; // number of cells to make

void drawHardTest1(void); // used for testing
void makeHardTest(int test);
int checkHardCorrectness(int level);
uint32_t rando();
int cmpfunc (const void * a, const void * b);
void drawLevel4(void);
