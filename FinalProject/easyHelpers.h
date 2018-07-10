/************************ 
 * finalAppHelpers.h                
 *         
 *   Authors: Nick Palumbo    
 *   Date Created: 10 Apr. 2016 
 *   Date Last Modified: 14 Apr. 2016  
 *   Assignment: FinalProject  
 *   Part of: Final Project       
 */



extern int currentCell; // current cell selected
extern int lastCell;  // last cell selected
extern nunchuk_t nunchukData; // used to read nunchuck data
extern int currentLevel;
extern int lastLevel;
  
extern int cellsOn[];

// 0        1       2       3        4          5      6        7  
// BLACK // BLUE // CYAN // GREEN // MAGENTA // RED // WHITE // YELLOW 
//int colorSelect = 0;

extern int x;
extern int y;

void drawCell(int x,int y, uint16_t color);
void drawHorizontalLine(int x, int y, uint16_t color);
void drawVerticalLine(int x, int y, uint16_t color);
void drawSelector(int x, int y);
void removeSelector(int x, int y);
void createCellData(void);
void ledTimer(void);
void drawCells(uint16_t color);
void printOnCells(void);


