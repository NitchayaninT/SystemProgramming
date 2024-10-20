#include <stdio.h>

//copy 1 byte at a time to "file.out"
int main(void)
{
  char c;
  FILE *in, *out;
  in = fopen("file.in", "rt");
  out = fopen("file.out","wt");

  while((c=getc(in)) != EOF) 
    //write to file.out byte by byte
    putc(c,out);
 
}