/************************
 * hardHelpers.h
 *        
 *   Authors: Nick Palumbo         
 *   Date Created: 10 Apr. 2016     
 *   Date Last Modified: 14 Apr. 2016    
 *   Assignment: FinalProject           
 *   Part of: Final Project          
 */

void drawHardCell(int x,int y, uint16_t color);
void drawHardHorizontalLine(int x, int y, uint16_t color);
void drawHardVerticalLine(int x, int y, uint16_t color);
void drawHardSelector(int x, int y);
void removeHardSelector(int x, int y);
void createHardCellData(void);
void ledHardTimer(void);
void drawHardCells(uint16_t color);
void printHardOnCells(void);
void readHardNunchukData(void);
void readHardUserButton(void);
