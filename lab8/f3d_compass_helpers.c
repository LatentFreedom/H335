/********************************************************** 
 * f3d_compass_helpers.c
 * 
 * Author: Nick Palumbo, Tim
 *   Date Created: March 4, 2016
 *   Last Modified by: Nick Palumbo
 *   Date Last Modified: March 10, 2016
 *   Assignment: lab8
 *   Part of: lab8
 */

#include <f3d_lcd_sd.h>

// draw direction label and return current direction
int calculateDirection(float heading, int currentDirection) {
  char direction[10];
  float angle =  heading;

  if(angle > 60 && angle <= 120){ // North 60...120
    f3d_lcd_drawString(10,150,"North",BLACK,WHITE);
    currentDirection = 0;
  }
  
  if(angle > 120 && angle <= 150){ // NorthWest 120...150
    f3d_lcd_drawString(10,150,"NorthWest",BLACK,WHITE);
    currentDirection = 1;
  }
  
  if(angle > 150 && angle <= 210){ // West 150...210
    f3d_lcd_drawString(10,150,"West",BLACK,WHITE);
    currentDirection = 2;
  }

  if(angle > 210 && angle <= 240){ // SouthWest 210...240
    f3d_lcd_drawString(10,150,"SouthWest",BLACK,WHITE);
    currentDirection = 3;
  }
  
  if(angle > 240 && angle <= 300){ // South 240...300
    f3d_lcd_drawString(10,150,"South",BLACK,WHITE);
    currentDirection = 4;
  }
  
  if(angle > 300 && angle <= 330){ // SouthEast 300...330
    f3d_lcd_drawString(10,150,"SouthEast",BLACK,WHITE);
    currentDirection = 5;
  }

  if(angle > 330 || angle <= 30){ // East 330...360 || 0...30
    f3d_lcd_drawString(10,150,"East",BLACK,WHITE);
    currentDirection = 6;
  }
  
  if(angle > 30 && angle <= 60){ // NorthEast 30...60
    f3d_lcd_drawString(10,150,"NorthEast",BLACK,WHITE);
    currentDirection = 7;
  }

  return currentDirection;
  
}

// draw circle
void lcd_drawCircle(){
  int radius = 50;         // set the radius
  int origin[2] = {65,90}; // set midpoint of circle
  int x, y;

  for(y = -radius; y<=radius;y++) // loop through y for length of diameter
    for(x=-radius;x<=radius;x++) // loop though x for length of diameter
      if(x*x+y*y>=radius*radius-100 && x*x+y*y<=radius*radius) 
	f3d_lcd_drawPixel(origin[0]+x,origin[1]+y,BLACK);
}


// draw marker on circle
void lcd_drawMarker(int direction){
  
}

// turn on LED given the direction
void led_drawMarker(int currentDirection){
  switch(currentDirection){
  case 0: f3d_led_on(0); break; // N
  case 1: f3d_led_on(1); break; // NE
  case 2: f3d_led_on(2); break; // E
  case 3: f3d_led_on(3); break; // SE
  case 4: f3d_led_on(4); break; // S
  case 5: f3d_led_on(5); break; // SW
  case 6: f3d_led_on(6); break; // W
  case 7: f3d_led_on(7); break; // NW
    }
}

// draw pitch/roll visual ball rolling given pitch and roll
void lcd_drawMarble(float pitch, float roll){
  int radius = 10;         // set the radius
  int origin[2]; // set midpoint of circle
  int x, y;
  origin[0] = (int)(roll * 30 * -1) + 65; // set x midpoint to be the roll
  origin[1] = (int)(pitch * 30 * -1) + 90; // set the y midpoint to be the pitch

  for(y = -radius; y<=radius;y++) // loop through y for length of diameter
    for(x=-radius;x<=radius;x++) // loop though x for length of diameter
      if(x*x+y*y>=radius*radius-18 && x*x+y*y<=radius*radius) 
	f3d_lcd_drawPixel(origin[0]+x,origin[1]+y,RED);
  delay(60);
  for(y = -radius; y<=radius;y++) // loop through y for length of diameter
    for(x=-radius;x<=radius;x++) // loop though x for length of diameter
      if(x*x+y*y>=radius*radius-18 && x*x+y*y<=radius*radius) 
	f3d_lcd_drawPixel(origin[0]+x,origin[1]+y,WHITE);
}

// remove the marble and led light
void lcd_clearDirection(){
  int x,y;
  for(x = 10;x<70;x++) // increase x 
    for(y = 150;y<160;y++) // increase y 
      f3d_lcd_drawPixel(x,y,WHITE);

  f3d_led_all_off();
  
}
