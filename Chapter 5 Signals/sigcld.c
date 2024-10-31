#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

static void sig_cld(int);

int main()
{
  pid_t pid[3];//fork 3 times has 14 processes running
  for(int i=0;i<3;i++)
    {
      pid[i]=fork();
    
  if (signal(SIGCLD, sig_cld) == SIG_ERR) //if has SLGCLD signal, go to function sig_cld which is a signal handler
    perror("signal error");
  if ((pid[i] = fork()) < 0) 
    perror("fork error");
  else if (pid[i] == 0) { /* child */ 
    sleep(2); 
    _exit(0);
  }
  pause(); /* when parent.waiting for signal */
    }
  exit(0);
}
static void sig_cld(int signo)
/* interrupts pause() */
{
  pid_t pid;
  int status;
  printf("SIGCLD received\n");
  /* reestablish handler in case you have many children */
  if (signal(SIGCLD, sig_cld) == SIG_ERR) //if dont have more SIGCLD, just skip this
    perror("signal error");
  /* fetch child status */
  if ((pid = wait(&status)) < 0) //returns child process to pio
    perror("wait error");
  printf("pid = %d\n", pid);
return;
}