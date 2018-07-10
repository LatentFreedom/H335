
/************************
 * queue.c
 *
 *   Authors: Curtis Bitner, Nick Palumbo
 *   Date Created: 01 Apr. 2016
 *   Date Last Modified: 01 Apr. 2016
 *   Assignment: Lab 11, Part 4
 *   Part of: Lab 4
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include "queue.h"

// initialize the queue
void init_queue(queue_t *buf) {
  buf->head=0; // set the head of the list to 0
  buf->tail=0; // set the tail of the list to 0
}

// add int to the buffer
int enqueue (queue_t *buf, int data) {
  
  // return 0 when the buffer does not have room for character
  if(buf->head == QUEUE_SIZE - 1) return 0;
  
  buf->buffer[buf->head] = data;    // put data in the current head

  // set the new head to the next head
  buf->head = (buf->head + 1) % QUEUE_SIZE; 

  // return 1 when character is successfully added
  return 1;

 
}

// remove character from the buffer
int dequeue (queue_t *buf) {
  
  // return 0 if the buffer is empty
  if(queue_empty(buf)) return 0; 
  
  // initialize the integer taken from queue
  int out = buf->buffer[buf->tail];
 
  // set the new tail
  buf->tail = (buf->tail + 1) % QUEUE_SIZE;
  return out; // return the data removed
}

// check if queue is empty
int queue_empty(queue_t *buf) {
  if(buf->head == buf->tail) return 1; // return 1 if queue is empty
  return 0; // return 0 if queue is not empty
}


/* queue.c ends here */
