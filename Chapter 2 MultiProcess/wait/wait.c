#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
pid_t pid; char *msg; int n;

printf("fork program starting\n");
pid = fork();
switch(pid)
{
  case -1: _exit(1);
  case 0: msg = "Child!\n"; n = 45; break;
  default: msg = "Parent!\n"; n = 3; break;
}

for (; n>0; n--) { 
  puts(msg); 
  sleep(1);
}
  //added
  if (pid) // if(pid!=0), if its a parent (wait for child to finish)
  {
    int stat_val; //status value
    pid_t child_pid; //id of the child
    
    child_pid = wait(&stat_val); //block, not doing anything 
    //until it gets  a stat_val signal from a child

    //child finishes, sends signal to parent
    printf("Child has nished: PID = %d\n",
    child_pid);
    
    if (WIFEXITED(stat_val))//check whether a status is a normal value
    printf("Child exited with code %d\n", WEXITSTATUS(stat_val));//return pid of the child
    else
    printf("Child terminated abnormally\n");
    _exit(0);
  }
  else _exit(15);

  
  //_exit(0);
}
