/************************
 * f3d_timer2.h
 *
 *   Authors: Tim Carlson, Nick Palumbo
 *   Date Created: 08 Apr. 2016
 *   Date Last Modified: 08 Apr. 2016
 *   Assignment: Lab 12
 *   Part of: Lab12
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include <stm32f30x.h>

void f3d_timer2_init(void);
void f3d_timer2_setperiod(uint16_t period);
void f3d_timer2_enable(uint32_t samplerate);
void f3d_timer2_disable(void);

/* f3d_timer2.h ends here */
