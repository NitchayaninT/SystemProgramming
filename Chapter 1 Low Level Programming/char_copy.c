#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//copy 1 byte at a time to "file.out"
int main(void)
{
  char c;int in,out;
  in = open("file.in",O_RDONLY);
  out = open("file.out",O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  while(read(in, &c, 1)==1) write(out,&c,1);
}