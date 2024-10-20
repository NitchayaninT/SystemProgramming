#include <unistd.h>
int main(void)
{
  if((write(1, "Some data is writen\n",21))!=21)
    write(2,"an error has occured on the stdout\n",36);
  return 0;

  
}