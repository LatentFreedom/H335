/********************************************************** 
 * main.c
 * 
 * Author: Nick Palumbo
 *   Date Created: March 20, 2016
 *   Last Modified by: Nick Palumbo
 *   Date Last Modified: March 20, 2016
 *   Assignment: lab9
 *   Part of: lab9
 */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_nunchuk.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <f3d_delay.h>
#include <math.h>
#include <f3d_gyro.h>

#define M_PI 3.14159265358979323846
#define TIMER 20000
#define BAR_HEIGHT 40

//Calculates pitch from acc data
float pitch (float xyz[]);
//Calculates roll from acc data
float roll (float xyz[]);
//Calculates heading adjusted for accelerometer from mag and acc data
float heading (float xyz[], float pitch, float roll /*float* headingData*/);

float pitch (float xyz[]) {
  float pitch;
  pitch = atan2( xyz[0] , sqrt((xyz[1] * xyz[1]) + (xyz[2] * xyz[2])));
  return pitch;
}

float roll (float xyz[]) {
  float roll;
  roll = atan2( xyz[1] , sqrt((xyz[0] * xyz[0]) + (xyz[2] * xyz[2])));
  return roll;
}



float heading (float xyz[], float pitch, float roll/* float* headingData*/){
  //float headingData[3];
  float xh, yh;
  xh = xyz[0] * cos(pitch) + xyz[2] * sin(pitch);
  yh = xyz[0] * sin(roll) * sin(pitch) + xyz[1] * cos(roll) - xyz[2] * sin(roll) * cos(pitch);
  float headingVal;
  headingVal = atan2(yh,xh);
  // headingData[0] = heading;
  //headingData[1] = xh;
  //headingData[2] = yh;
  return headingVal;
}

// Draws rectangle downwards
void drawrectangledown(int x,int y, int w, int l, int color){
  while(l>0){
    int wd=w;
    int xd=x;
    while(wd>0){
      f3d_lcd_drawPixel(xd,y,color);
      xd++;
      wd--;
    }
    y++;
    l--;}
}

// draws rectangle upwards
void drawrectangleup(int x,int y, int w, int l, int color){
  while(l>0){
    int wd=w;
    int xd=x;
    while(wd>0){
      f3d_lcd_drawPixel(xd,y,color);
      xd++;
      wd--;
    }
    y--;
    l--;}
}


// Draws graphs for accelerometer data (pitch and roll)
void drawbargraph(int i, int read){
  int l = read;
  if(l>0){
    switch(i){
    case 0: drawrectangleup(35,90,20,l,RED);break;
    case 1: drawrectangleup(75,90,20,l,GREEN);break;
    case 2: drawrectangleup(35,90,20,l,BLACK);break;
    case 3: drawrectangleup(75,90,20,l,BLACK);break;
    }
  }else{
    l=-l;
    switch(i){
    case 0: drawrectangledown(35,90,20,l,RED);break;
    case 1: drawrectangledown(75,90,20,l,GREEN);break;
    case 2: drawrectangledown(35,90,20,l,BLACK);break;
    case 3: drawrectangledown(75,90,20,l,BLACK);break;
    }
  }
}// Code borrowed from MIT site
// http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
// Draws points for circle with eight point symmetry
void circlePoints(int cx, int cy, int x, int y, int col) {
  if (x == 0) {
    f3d_lcd_drawPixel(cx, cy + y, col);
    f3d_lcd_drawPixel(cx, cy - y, col);
    f3d_lcd_drawPixel(cx + y, cy,col);
    f3d_lcd_drawPixel(cx - y, cy, col);
  } else
    if (x == y) {
      f3d_lcd_drawPixel(cx + x, cy + y, col);
      f3d_lcd_drawPixel(cx - x, cy + y, col);
      f3d_lcd_drawPixel(cx + x, cy - y, col);
      f3d_lcd_drawPixel(cx - x, cy - y, col);
    } else
      if (x < y) {
	f3d_lcd_drawPixel(cx + x, cy + y, col);
	f3d_lcd_drawPixel(cx - x, cy + y, col);
	f3d_lcd_drawPixel(cx + x, cy - y, col);
	f3d_lcd_drawPixel( cx - x, cy - y, col);
	f3d_lcd_drawPixel(cx + y, cy + x, col);
	f3d_lcd_drawPixel( cx - y, cy + x, col);
	f3d_lcd_drawPixel(cx + y, cy - x, col);
	f3d_lcd_drawPixel(cx - y, cy - x, col);
      }
}

// Borrowed from MIT site
// http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
// Uses the circlePoints function to draw circle
void circleMidpoint(int xCenter, int yCenter, int radius, int col) {
  int x = 0;
  int y = radius;
  int p = (5 - radius*4)/4;
  circlePoints(xCenter, yCenter, x, y, col);
  while (x < y) {
    x++;
    if (p < 0) {
      p += 2*x+1;
    } else {
      y--;
      p += 2*(x-y)+1;
    }
    circlePoints(xCenter, yCenter, x, y,col);
  }
}

void drawRect(uint16_t color, int x, int y, int width, int height) {
  int curX = x;
  int curY = y;

  while (curX < x + width) {
    while (curY < y + height) {
      f3d_lcd_drawPixel(curX, curY, color);
      curY++;
    }
    curX++;
    curY = y;
  }
}

void drawBar(int rate, int pos) {
  int x;

  rate /= 10; // cuz 600 is the maximum
  if (rate > 0) {
    x = 64;
  } else {
    rate *= -1;
    x = 64 - rate;
  }

  switch (pos) {
  case 0:
    drawRect(RED, x, 20, rate, BAR_HEIGHT);
    delay(5); // I think this makes it look a bit clearer
    drawRect(BLACK, x, 20, rate, BAR_HEIGHT); //repaint
    break;
  case 1:
    drawRect(GREEN, x, 60, rate, BAR_HEIGHT);
    delay(5);
    drawRect(BLACK, x, 60, rate, BAR_HEIGHT);
    break;
  case 2:
    drawRect(BLUE, x, 100, rate, BAR_HEIGHT);
    delay(5);
    drawRect(BLACK, x, 100, rate, BAR_HEIGHT);
    break;
  default:
    break;
  }
}


// allows user choice between board accel, magnetometer, gyro, and nunchuk accel
int main(void) {
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_user_btn_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  f3d_gyro_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_lcd_fillScreen(BLACK);
  nunchuk_t nunchukData;// = {0,0,0,0,0,0};
  float accelData[3];
  float oldAccelData[3] = {0,0,0};
  float magData[3];
  float headingData[3];
  int x = 0, holdingbutton = 0;
  float gyroData[3] = { 0, 0, 0 };

  while(1) {
    f3d_nunchuk_read(&nunchukData);

    //CHECK IF SWITCH DISPLAY----------------------------------*/
    if(nunchukData.c || (nunchukData.jx < 100)) {
      if (!(holdingbutton == 1)) {
	if (x == 3) {x = 0;}
	else {x = x + 1;}
	f3d_lcd_fillScreen(BLACK);
	holdingbutton = 1;
      }
    } else if (nunchukData.z || (nunchukData.jx > 180)) {
      if (!(holdingbutton == 2)) {
	if (x == 0) {x = 3;}
	else {x = x - 1;}
	f3d_lcd_fillScreen(BLACK);
	holdingbutton = 2;
      }
    }
    else {
      holdingbutton = 0;
    }
    //CHECK END------------------------------------------------*/

    //setup for nunchuk accel display--------------------------*/
    oldAccelData[0] = accelData[0];
    oldAccelData[1] = accelData[1];
    oldAccelData[2] = accelData[2];
    f3d_accel_read(accelData);
    //SETUP END -----------------------------------------------*/

    //setup for mag display
    f3d_mag_read(magData);


    /*setup for nunchuk accel display---------------------*/
    f3d_nunchuk_read(&nunchukData);

    float nunx = ((nunchukData.ax - 511.5) / 511.5);
    float nuny = ((nunchukData.ay - 511.5) / 511.5);
    float nunz = ((nunchukData.az - 511.5) / 511.5);
    float nunchukAccelData[3];
    float oldNunchukAccelData[3] = {0,0,0};//int x = 0;
    nunx = ((nunchukData.ax - 511.5) / 511.5);
    nuny = ((nunchukData.ay - 511.5) / 511.5);
    nunz = ((nunchukData.az - 511.5) / 511.5);
    oldNunchukAccelData[0] = nunchukAccelData[0];
    oldNunchukAccelData[1] = nunchukAccelData[1];
    oldNunchukAccelData[2] = nunchukAccelData[2];

    nunchukAccelData[0] = nunx;
    nunchukAccelData[1] = nuny;
    nunchukAccelData[2] = nunz;
    /*SETUP END--------------------------------------------*/

    // BOARD ACCELEROMETER---------------------------------*/
    if (x == 0) {
      f3d_lcd_drawString(10,10,"board accel",WHITE,BLACK);
      //accelDisplay();
      printf("1\n");
      printf("%d\n", x);
      int i;
      for (i=0; i < 1; i++){
	drawbargraph(2,(int) (pitch(oldAccelData) * 30));
	drawbargraph(0,(int) (pitch(accelData) * 30));
	drawbargraph(3,(int) (roll(oldAccelData) * 30));
	drawbargraph(1,(int) (roll(accelData) * 30));


	// draws the line between the top bargraph and the bottom bar graph
	int line=114;
	while(line>14){
	  f3d_lcd_drawPixel(line,90,WHITE);
	  line--;
	}
	//draws a "0" by the bar graph
	f3d_lcd_drawString(7,88,"0", WHITE,BLACK);
	f3d_lcd_drawString(44,142,"X", WHITE,BLACK);
	f3d_lcd_drawString(84,142,"Y",WHITE,BLACK);

      }
    }
    //BOARD ACCEL END--------------------------------------*/

    // LOOK AT THIS COMPASS--------------------------------*/
    else if (x == 1) {
      printf("2\n");
      f3d_lcd_drawString(10,5,"magnetometer",YELLOW,BLACK);
      float headingVal = heading(magData, pitch(accelData), roll(accelData));
      // Adjust reading for north value
      if (headingVal > (M_PI / 2)) {
	headingVal = ((headingVal + (M_PI / 2)) - M_PI) - M_PI;
      }else{
	headingVal = headingVal + (M_PI / 2);
      }

      f3d_lcd_drawString(64,15,"N", YELLOW ,BLACK);
      f3d_lcd_drawString(64,135,"S", YELLOW, BLACK);
      f3d_lcd_drawString(5,80,"W", YELLOW ,BLACK);
      f3d_lcd_drawString(120,80,"E", YELLOW ,BLACK);
      circleMidpoint(64, 80,50,RED);
      float x1,y1;
      // x1 and y1 give us a point on the circumference of a smaller radius circle with same center
      x1 = (45 * cos(headingVal)) + 64;
      y1 = (45 * sin(headingVal)) + 80;
      drawrectangledown(x1-1,y1-1,3, 3, GREEN);


      delay(100);
      drawrectangledown(x1-1,y1-1, 3, 3, BLACK);
      printf("Mag: %f", magData);
    }
    //MAG END---------------------------------------------*/

    //THIS IS THE GYRO DISPLAY----------------------------*/
    else if (x == 2) {
      f3d_lcd_drawString(10,10,"gyro",WHITE,BLACK);
      printf("3\n");
      f3d_gyro_getdata(gyroData);
      int i;
      for (i = 0; i < 3; i++) {
	drawBar((int) gyroData[i], i);
	printf("%f\n", gyroData[i]);

      }
    }
    //GYROEND---------------------------------------------*/


    //THIS IS THE NUNCHUK ACCELEROMETER DISPLAY-----------*/
    else if (x == 3) {
      f3d_lcd_drawString(10,10,"nunchuk accel",WHITE,BLACK);
      printf("4\n");

      int i;
      for (i=0; i < 1; i++){
	drawbargraph(2,(int) (pitch(oldNunchukAccelData) * 30));
	drawbargraph(0,(int) (pitch(nunchukAccelData) * 30));
	drawbargraph(3,(int) (roll(oldNunchukAccelData) * 30));
	drawbargraph(1,(int) (roll(nunchukAccelData) * 30));


	// draws the line between the top bargraph and the bottom bar graph
	int line=114;
	while(line>14){
	  f3d_lcd_drawPixel(line,90,WHITE);
	  line--;
	}
	//draws a "0" by the bar graph
	f3d_lcd_drawString(7,88,"0", WHITE,BLACK);
	f3d_lcd_drawString(44,142,"X", WHITE,BLACK);
	f3d_lcd_drawString(84,142,"Y",WHITE,BLACK);

      }
    }
    else { printf("oh noooo, why am i hereeeee\n");}
  }
  //NUNCHUK ACCEL END------------------------------------*/
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
