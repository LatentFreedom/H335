/************************
 * screen.h
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

void fillScreen(uint16_t);
void drawChar(uint8_t, uint8_t, unsigned char, uint16_t, uint16_t);
void drawString(uint8_t, uint8_t, char *, uint16_t, uint16_t);
void drawRect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

/* screen.h ends here */
