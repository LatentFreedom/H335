/************************
 * selectScreen.h
 *        
 *   Authors: Nick Palumbo         
 *   Date Created: 10 Apr. 2016     
 *   Date Last Modified: 14 Apr. 2016    
 *   Assignment: FinalProject           
 *   Part of: Final Project          
 */

// extern nunchuk_t nunchukData; // used to read nunchuck data
// extern int currentLevel;
// extern int lastLevel;

void drawBox(int x,int y, int length, uint16_t color);
void redrawSelectionBox(int current, int previous);
void displayLevelSelectScreen(void);
