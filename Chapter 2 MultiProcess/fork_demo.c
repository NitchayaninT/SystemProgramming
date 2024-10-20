#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
pid_t pid; char *msg; int n;

printf("fork program starting\n");
pid = fork(); //creates a child
  //fork returns 0 = you're in a new child process
  //fork returns >1, you're back in parent
switch(pid) 
{
case -1: 
  _exit(1); //error, exit immediately, return some number
case 0: 
  msg = "Child!\n"; //pointer to thr word Child
  n = 45; //print out child 45 times
  break;
default: 
  msg = "Parent!\n"; 
  n = 3; //3 times
  break;
}

for (; n>0; n--) { 
  puts(msg); //output will be random order, maybe the first word outputs child or parent
  sleep(1); 
}
_exit(0);
}