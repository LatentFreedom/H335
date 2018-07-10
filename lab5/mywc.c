#include <stdio.h>

int main(int argc, const char * argv[])
{ 
  int line_count = 0, word_count = 0, character_count = 0;
  char c;

  while ((c = getchar()) != 0x1b) // end on Esc
  {
    character_count++;
    if(c == ' ')
      word_count++;
    else if(c == '\n')
    {
      word_count++;
      line_count++;
    }

  }
  
  printf("%d %d %d\n",line_count, word_count, character_count);
  return 0;
}
