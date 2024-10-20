#include <stdio.h>

//copy 1 byte at a time to "file.out"
int main(void)
{
  char block[512];
  FILE *in, *out;
  in = fopen("file.in", "rt");//read text file
  out = fopen("file.out","wt");

  while(fgets(block,sizeof(block),in) != NULL) 
    //write to file.out 50 bytes at a time
    fputs(block,out);

  fclose(in);
  fclose(out);

}