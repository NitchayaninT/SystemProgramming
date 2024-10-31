#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void rx_int(int sig)
{ 
  printf("SIGINT - %d Received!!!\n", sig); 
}

int main(void)
{
  struct sigaction act;
  
  act.sa_handler = rx_int;
  sigemptyset(&act.sa_mask); //sa_mask=set of signals
  
  //can block the signal
  sigaddset(&act.sa_mask, SIGINT);
  //sigdelset = deletes SIG from the set of signals
  sigaddset(&act.sa_mask, SIGUSR1);
  sigaddset(&act.sa_mask, SIGUSR2);
  sigprocmask(SIG_BLOCK, &act.sa_mask, NULL); //blocks all these 3 signals above
  //you can also unblock like sigprocmask(SIG_UNBLOCK, &act.sa_mask,NULL)
  act.sa_flags = 0;
  //for POSIX, we can handle many signals. assume everything's in one struct variable act.sa_mask to block, unblock...
  //if we do this normally not POSIX, just use "signal(SIGNAL, rx_int)";
  
  sigaction(SIGINT, &act, 0); //catching SIGINT signal
  
  while(1) { 
    printf("Running...\n"); 
    sleep(1); 
    }
}