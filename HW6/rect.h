/************************
 * rect.h
 *
 *   Authors: Nick Palumbo
 *   Date Created: 12 Apr. 2016
 *   Date Last Modified: 12 Apr. 2016
 *   Assignment: HW6
 *   Part of: HW6
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include "c335sim.h"

#define COLLISION_TOP 1
#define COLLISION_BOTTOM 2
#define COLLISION_LEFT 3
#define COLLISION_RIGHT 4

typedef struct rect {
  uint8_t pos_x;
  uint8_t pos_y;
  uint8_t width;
  uint8_t depth;
  uint16_t color;
} rect_t;

void initRect(rect_t *, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
void eraseRect(rect_t *, uint16_t);
void redrawRect(rect_t *);
int moveRect(rect_t *, int8_t, int8_t, uint16_t);



/* rect.h ends here */
