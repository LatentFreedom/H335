/************************
 * tests.h
 *
 *   Authors: Nick Palumbo
 *   Date Created: 10 Apr. 2016
 *   Date Last Modified: 14 Apr. 2016
 *   Assignment: FinalProject
 *   Part of: Final Project
 */

int easyPoints[32][2] = {{0,  0},{16,   0},{32,  0},{48,  0},{64,  0},{80,  0},{96,  0},{112,  0},
			 {0, 40},{16,  40},{32, 40},{48, 40},{64, 40},{80, 40},{96, 40},{112, 40},
			 {0, 80},{16,  80},{32, 80},{48, 80},{64, 80},{80, 80},{96, 80},{112, 80},
			 {0,120},{16, 120},{32,120},{48,120},{64,120},{80,120},{96,120},{112,120}};

int test1[] = {0,7,24,31}; // used for testing

int *easyLevel = *easyPoints;
extern int cellCount; // number of cells to make

void drawTest1(void); // used for testing
void makeTest(int test);
int checkCorrectness(int level);
