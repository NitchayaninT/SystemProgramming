#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

/* one handler for both signals */
static void sig_usr(int);

int main(void)
{
  if (signal(SIGUSR1, sig_usr) == SIG_ERR) //tell system that whenever we recieve SIGUSR1, its gonna jump to function sig_usr
    //by sending SIGUSR signal to the process, we use kill -signalid pid to send the signal to that process
    perror("can't catch SIGUSR1");
  if (signal(SIGUSR2, sig_usr) == SIG_ERR)//tell system that whenever we recueieve SIGUSR2, go to th e same function
    perror("can't catch SIGUSR2");

  if (signal(SIGINT, sig_usr) == SIG_ERR)
    perror("can't catch SIGINT");
  for ( ; ; )//forever loop
    pause();
  //means program will not end. it will continue listening to the signal forever
}
/* argument is signal number */
static void sig_usr(int signo)
{
  if (signo == SIGUSR1)//compare if the signal is equivalent to SIGUSR1. you can write as signo == 10 as well.
    printf("received SIGUSR1\n");
  else if (signo == SIGUSR2)
    printf("received SIGUSR2\n");//if equivalent to SIGUSR2
  else 
    printf("received signal %d\n", signo);//if signal is smth else, 
}
