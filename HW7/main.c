/************************
 * main.c
 *
 *   Authors: Nick Palumbo
 *   Date Created: 17 Apr. 2016
 *   Date Last Modified: 17 Apr. 2016
 *   Assignment: HW7
 *   Part of: HW7
 */

/* Copyright (c) 2015-2016 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include <stdio.h>
#include <stdlib.h>

struct Node {
  int item;
  struct Node *next;
};

int listsum_recursive(struct Node *p) {
  if (p->next==NULL) {
    return (p->item);
  }
  else {
    return(p->item + listsum_recursive(p->next));
  }
}

extern listsum_recursive_asm (struct Node *);

int main() {
  int i;
  struct Node *p;
  struct Node *current;
  struct Node *head;
  head = NULL;

  for (i=1;i<10;i++) {
    current = (struct Node *) malloc(sizeof(struct Node));
    current->item = i;
    current->next = head;
    head = current;
  }
  printf("Recursive C Solution = %d\n",listsum_recursive(head));
  printf("Recursive ASM Solution = %d\n",listsum_recursive_asm(head));
}



/* main.c ends here */
