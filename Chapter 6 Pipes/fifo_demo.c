#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FIFO_NAME "/tmp/my_fifo"
int main(int argc, char *argv[])
{
  int res, open_mode = 0;
  if (argc < 2) //user needs to input one of these as argv
  {
    fprintf(stderr, "Usage: %s <[O_RDONLY, O_WRONLY, O_RDWR, O_NONBLOCK]>\n", *argv);
    exit(EXIT_FAILURE);
  }
  argv++; //argv[1]
  if (*argv) //user can choose one of these
  {
    if (strncmp(*argv, "O_RDONLY", 8) == 0)
    open_mode |= O_RDONLY;
    if (strncmp(*argv, "O_WRONLY", 8) == 0)
    open_mode |= O_WRONLY;
  
    if (strncmp(*argv, "O_RDWR", 6) == 0)
  
    open_mode |= O_RDWR;
    if (strncmp(*argv, "O_NONBLOCK", 10) == 0)
    open_mode |= O_NONBLOCK;
  }
  //check if the file doesnt already exist
  if (access(FIFO_NAME, F_OK) == -1)
  {
    res = mkfifo(FIFO_NAME, 0777);//create FIFO
    if (res)
    {
      fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
      exit(EXIT_FAILURE);
    }
  }
    printf("Process %d opening FIFO\n", getpid());
    res = open(FIFO_NAME, open_mode);//open file FIFO_NAME
    printf("Process %d result %d\n", getpid(), res);
    sleep(5);
    if (res != -1) 
      (void) close(res);
    printf("Process %d finished\n", getpid());
    unlink(FIFO_NAME); //after we use, we delete pipe from system (when we use ls -l /tmp, i cant see the file anymore)
    exit(EXIT_SUCCESS);
    }
  //echo "something" > /tmp/my_fifo to send something to the "pipe" /tmp/my_fifo
//another process will read from the pipe at another end.
//./fifo O_RDONLY = tell the process to read from pipe. so i have to send something to the pipe first (1st process) so that another process will read from it