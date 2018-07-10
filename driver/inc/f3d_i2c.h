/********************************************************** 
* f3d_i2c.h
* 
* Author: Nick Palumbo
*   Date Created: March 4, 2016
*   Last Modified by: Nick Palumbo
*   Date Last Modified: March 4, 2016
*   Assignment: lab8
*   Part of: lab8
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

void f3d_i2c1_init();
void f3d_i2c1_read(uint8_t device, uint8_t reg, uint8_t* buffer, uint16_t numbytes);
void f3d_i2c1_read_nunchuck (uint8_t device, uint8_t* buffer, uint16_t numbytes);
void f3d_i2c1_write(uint8_t device, uint8_t reg, uint8_t* value);


/* f3d_i2c.h ends here */
