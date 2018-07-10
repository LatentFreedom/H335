/************************
 * pong.c
 *
 *   Authors: Nick Palumbo
 *   Date Created: 12 Apr. 2016
 *   Date Last Modified: 12 Apr. 2016
 *   Assignment: HW6
 *   Part of: HW6
 */

// use to debug with command line arguments
// gdb --args pong portrait test.txt

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include "c335sim.h"
#include "rect.h"
#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#define PADDLE_MOVE_INCREMENT 3
#define PADDLE_HEIGHT 25
#define PADDLE_THICKNESS 3
#define BALL_DIM 3
#define EVENT_LOOP_TIME 20

rect_t left_paddle;
rect_t right_paddle;
rect_t top_paddle;
rect_t bottom_paddle;
rect_t ball;

int ball_vx = 1;
int ball_vy = 2;
char currentCoord[13] = "";

int orientation = 0; // portrait: 0 // landscape: 1

/*The event loop that handles the key input*/
void event_loop(char line[]) {
  static int paddle_left_move = 0; 
  static int paddle_right_move = 0;
  SDL_Event event;
  SDL_PollEvent(&event);

  if(orientation == 0) {
    switch(event.type) {
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
	paddle_right_move = 0;
	break;
      case SDLK_DOWN:
	paddle_right_move = 0;
	break;
      case SDLK_a:
	paddle_left_move = 0;
	break;
      case SDLK_z:
	paddle_left_move = 0;
	break;
      default:
	break;
      }
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
	paddle_right_move = -PADDLE_MOVE_INCREMENT;
	break;
      case SDLK_DOWN:
	paddle_right_move = PADDLE_MOVE_INCREMENT;
	break;
      case SDLK_a:
	paddle_left_move = -PADDLE_MOVE_INCREMENT; 
	break;
      case SDLK_z:
	paddle_left_move = PADDLE_MOVE_INCREMENT; 
	break;
      case SDLK_q:
	printf("Q PRESSED - Exit Program\n");
	exit(0);
	break;
	// add new coord from file list when n is pressed
      case SDLK_n:
	// copy current to global for redrawing 
	strcpy(currentCoord,line);
	drawString(20,20,line,WHITE,BLACK);
      default:
	break;
      }
      break;
    }
    if (paddle_left_move) {
      moveRect(&left_paddle, 0, paddle_left_move, BLACK);
    }
    if (paddle_right_move) {
      moveRect(&right_paddle, 0, paddle_right_move, BLACK);
    }
  } else { // landscape mode //
    switch(event.type) {
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
	paddle_right_move = 0;
	break;
      case SDLK_DOWN:
	paddle_right_move = 0;
	break;
      case SDLK_a:
	paddle_left_move = 0;
	break;
      case SDLK_z:
	paddle_left_move = 0;
	break;
      default:
	break;
      }
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_LEFT:
	paddle_right_move = -PADDLE_MOVE_INCREMENT;
	break;
      case SDLK_RIGHT:
	paddle_right_move = PADDLE_MOVE_INCREMENT;
	break;
      case SDLK_z:
	paddle_left_move = -PADDLE_MOVE_INCREMENT; 
	break;
      case SDLK_x:
	paddle_left_move = PADDLE_MOVE_INCREMENT; 
	break;
      case SDLK_q:
	printf("Q PRESSED - Exit Program\n");
	exit(0);
	break;
	// add new coord from file list when n is pressed
      case SDLK_n:
	// copy current to global for redrawing 
	strcpy(currentCoord,line);
	drawString(20,20,line,WHITE,BLACK);
      default:
	break;
      }
      break;
    }
    if (paddle_left_move) {
      moveRect(&top_paddle, paddle_left_move, 0, BLACK);
    }
    if (paddle_right_move) {
      moveRect(&bottom_paddle, paddle_right_move, 0, BLACK);
    }
  }

}

/*Where the collisions are handled*/
void pong_game(char line[]) {
  int collision;

  if(orientation == 0) {
    drawString(40,60,"hit q to quit",WHITE,BLACK);
    drawString(20,20,currentCoord,WHITE,BLACK);
    redrawRect(&left_paddle);
    redrawRect(&right_paddle);
    event_loop(line);
    collision = moveRect(&ball, ball_vx, ball_vy, BLACK);
  } else {
    drawString(40,60,"tiuq ot q tih",WHITE,BLACK);
    drawString(20,20,currentCoord,WHITE,BLACK);
    redrawRect(&top_paddle);
    redrawRect(&bottom_paddle);
    event_loop(line);
    collision = moveRect(&ball, ball_vx, ball_vy, BLACK);
  }

  switch (collision) {
  case COLLISION_TOP:
    ball_vy = -ball_vy;
    break;
  case COLLISION_BOTTOM:
    ball_vy = -ball_vy;
    break;
  case COLLISION_LEFT:
    ball_vx = -ball_vx;
    break;
  case COLLISION_RIGHT:
    ball_vx = -ball_vx;
    break;
  }
}

/*Where the pong_game() is called the rectangels are initialized. */
int c335_main( int argc, char *argv[] ) {

  fillScreen(BLACK);

  // check scape // add paddles in correct positions 
  if(strcmp("PORTRAIT",argv[1]) == 0 || strcmp("portrait",argv[1]) == 0) {
    initRect(&left_paddle,
	     0,
	     ST7735_height/2-(PADDLE_HEIGHT/2),
	     PADDLE_THICKNESS,PADDLE_HEIGHT,WHITE);
    initRect(&right_paddle,
	     ST7735_width-PADDLE_THICKNESS,
	     ST7735_height/2-(PADDLE_HEIGHT/2),
	     PADDLE_THICKNESS,
	     PADDLE_HEIGHT,WHITE);
    initRect(&ball,
	     ST7735_width/2-(BALL_DIM/2),
	     ST7735_height/2-(BALL_DIM/2),
	     BALL_DIM,
	     BALL_DIM,WHITE);
    orientation = 0;
  } else {
    initRect(&top_paddle,
	     50,
	     0,
	     PADDLE_THICKNESS,
	     PADDLE_HEIGHT,
	     WHITE);
    initRect(&bottom_paddle,
	     50,
	     ST7735_height + 28,
	     PADDLE_THICKNESS,
	     PADDLE_HEIGHT,
	     WHITE);
    initRect(&ball,
	     ST7735_width/2-(BALL_DIM/2),
	     ST7735_height/2-(BALL_DIM/2),
	     BALL_DIM,
	     BALL_DIM,
	     WHITE);
    orientation = 1;
  }

  FILE *inputFile;
  
  // check if second arg is actually a file
  if(strstr(argv[2], ".txt")) { // if argv[2] contains string .txt
    inputFile = fopen(argv[2],"r"); // r for "read"
  } else {
    printf("File input not valid.\n");
    return 0;
  }

  // check if file could be opened
  if(inputFile == 0) {
    printf("Failed to open file.\n");
    return 0;
  }

  int i = 0; // counter
  char line[13]; // string to hold a line read from input file
  char array[30][20]; // array of strings

  while (fgets(array[i], sizeof(line), inputFile)) {
    // remove the '\n' on the end of each string
    // fgets ends on '\n', buf size, or 'eof'
    array[i][11] = '\0';
    i++;
  }
  fclose(inputFile);
  
  i = 0;
  char temp[15];

  while (1) {
    strcpy(temp, array[i]);       // copy string from array to temp
    i = (i + 1) % 16;             // increment i // % by amount of lines in file
    pong_game(array[i]);
    Delay(EVENT_LOOP_TIME);
    } 
} 

/* pong.c ends here */
