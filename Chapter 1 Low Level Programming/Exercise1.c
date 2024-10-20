#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
  char c;
  char block[5000];
  int in, out, nread;
  in = open("holeinfile.in", O_RDONLY);
  out = open("holeinfile.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  int filelength = 0;
  while ((nread = read(in, block, sizeof(block))) > 0) { // returns size of file.in
    write(out, block, nread);
  }

  int midpoint = lseek(in, 0, SEEK_END) / 2;
  lseek(out, midpoint - 8, SEEK_SET);
  write(out, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16);

  return 0;
}