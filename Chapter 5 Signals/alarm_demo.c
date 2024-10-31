#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void my_alarm(int sig)
{ 
  for (int i = 0; i < 5; i++) 
  printf("Alarm!!! \n"); }

int main(void)
{
int pid;
printf("Alarm clock is starting...\n");

/*if ((pid = fork()) == 0)//if child
{ 
  sleep(3); 
  kill(getppid(), SIGALRM); //send signal to the parent (getppid())
  exit(0); 
}*/

  
printf("Waiting for alarm...\n"); //if parent, prints wating for alarm
signal(SIGALRM, my_alarm);//once recieve the signal SIGALRM, JUMP to function my_alarm and print alarm 5 times(parent do that) = "funciton handler"
alarm(3); //wait for 3 seconds and then send signal SIGALRM. send signal to ourseleves (no need to create child). can replace this to the condition above
pause(); //if not recive anything, pause. suspend calling process until signal is recieved


printf("Done!\n"); 
exit(0);
}
  