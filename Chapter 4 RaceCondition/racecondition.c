#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
  pid_t pid; 
  char *msg, *omsg, c; 
  int n, turn = 0,flagfd;
  //avoiding race condition
  flagfd = open("flag",O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
  write(flagfd, &turn, 1); //stores turn=0 in flagfd first
    
  printf("fork program starting\n");
  pid = fork();
  
  switch(pid)
  {
  case -1: 
    exit(1);
  case 0: 
    msg = "Child"; 
    n = 35; 
    break;
  default: 
    msg = "Parent"; 
    n = 35; 
    break;
  }
  setbuf(stdout, NULL); //we want the char to immediately print out of the screen, not wait until buffer is full
  omsg = msg; //omsg = beginning of file
    for (; n>0; n--)
    {
      if (pid) //if parent
      {
        while (turn) //turn =1, parent's turn to run
        { 
          sleep(1); //wait to take turn, otherwise it will be too fast
          lseek(flagfd, 0, SEEK_SET);//moves pointer back to beginning of the file
          read (flagfd, &turn, 1); 
        }
      }
      else //child
      {
        while (!turn) //while turn = 0, child's turn to run
        { 
          sleep(1); 
          lseek(flagfd, 0, SEEK_SET);
          read (flagfd, &turn, 1); 
        }
    }
      
    while (c = *msg++) //msg = pointer to string c
      putc(c, stdout);
    msg = omsg;//make pointer point back to beginning of string c
    if (pid) { //parent
        turn = 1; 
        lseek(flagfd, 0, SEEK_SET);
        write (flagfd, &turn, 1); 
      }
    else { //child
      turn = 0; 
      lseek(flagfd, 0, SEEK_SET);
      write (flagfd, &turn, 1); 
    }
  }
    return 0;
/*setbuf(stdout, NULL); // set unbuffered. means we dont want buffering
  omsg = msg; //omsg points the beginning of the message
  for (; n>0; n--)
  {
    while(c = *msg++) //runs until msg points to NULL
    {
      putc(c, stdout); 
      sleep(1);
    }
    msg = omsg; //points back to the beginning of the message (restore)
  } 
  if (pid)
  {
    int stat_val; pid_t child_pid;
    child_pid = wait(&stat_val);
    printf("Child has nished: PID = %d\n", child_pid);
  if (WIFEXITED(stat_val))
    printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
  else
    printf("Child terminated abnormally\n");
  }
  exit(0);*/
  }
