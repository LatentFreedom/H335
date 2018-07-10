/********************************************************** 
* lcd_app_helpers.c
* These functions help with main.c when creating visuals on the lcd
* Author: Nick Palumbo, Ethan Lawton
*   Date Created: February 28, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: March 2, 2016
*   Assignment: HW6
*   Part of: HW6
*/


#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <c335sim.h>
#include <string.h> // used for sprintf



// clear screen with given color
void clearScreen(uint16_t color) {
  fillScreen(color);
}

// draw bar for each axis
void drawBars(int x, int y, int z) {
  drawRect(40,0,20,x,RED); // x-axis bar
  drawRect(60,0,20,y,BLUE); // y-axis bar
  drawRect(80,0,20,z,GREEN); // z-axis bar
}

// remove axis speed line for next line to be shown
void removeLine(int x, int y, uint16_t color) {
  int i;
  for(i=x;i<(x+20);i++){
    // putpixel(i,y,color);
  }
}

void drawLabels(char x[],char y[], char z[]) {

  drawChar(1,0,'X',BLACK,WHITE);
  drawString(10,0,x,BLACK,WHITE);

  drawChar(1,10,'Y',BLACK,WHITE);
  drawString(10,10,y,BLACK,WHITE);
  
  drawChar(1,20,'Z',BLACK,WHITE);
  drawString(10,20,z,BLACK,WHITE);
}

/*The event loop that handles the key input*/
int event_loop(char x[], char y[], char z[], int xi, int yi, int zi) {
  SDL_Event event;
  SDL_PollEvent(&event);

    switch(event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_q:
	printf("Q PRESSED - Exit Program\n");
	exit(0);
	break;
	// add new coord from file list when n is pressed
      case SDLK_n:
	fillScreen(WHITE);
	drawLabels(x,y,z);
	drawBars(xi,yi,zi);
	break;
      default:
	break;
      }
    }
}
