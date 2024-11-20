//Pitchapa Phisutpichet 6580065
//Patrapee Maleevech 6580074
//Nitchayanin Thamkunanon 6580081
//Chanakan Boonchoo 6580128
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#define MEM_SIZE 4096
//parent producer child consumer
int shmid;
void signal_handler(int SIGNO){
  shmctl(shmid, IPC_RMID, 0);
  exit(0);
}
struct shm_st
{
  int written;
  int turn;
  //handle race condition
  //turn 1 = user1->user2. turn 2 = user2->user1
  char data[BUFSIZ];
};
int main(int argc, char*argv[])
{
  int running = 1, shmID;
  void *sh_mem;
  struct shm_st *sh_area;
  char buffer[BUFSIZ];
  if (argc < 2) 
  {
    fprintf(stderr, "Usage: %s User has to enter 1 or 2 as argv\n", *argv);
    exit(EXIT_FAILURE);
  }
  argv++;

  //CREATING SHARE MEMORY
   shmid = shmget((key_t) 21930, MEM_SIZE, 0666 | IPC_CREAT); 

  //ROLES
  //user 1. parent sends to sm 1. child consumes from sm2
  //user 2. parent sends to sm2. child consumes from sm1
  if (strcmp(*argv, "1") == 0) //user 1
  {
    pid_t pid;
    pid = fork();
    sh_mem = shmat(shmid, NULL, 0);
    sh_area = (struct shm_st *) sh_mem;
 
    
    if(pid>0)//parent produces
    {
      signal(SIGTERM,signal_handler);
      while (running)
      {
        while (sh_area->turn == 2 && sh_area->written == 0)
        {
          sleep(1); 
          //printf("Waiting...\n");
        }
        fgets(buffer, BUFSIZ, stdin);
        strcpy(sh_area->data, buffer);
        sh_area->written = 1;
        sh_area->turn = 1;
        if (strncmp(buffer, "end chat", 8) == 0)
          {
            running = 0;
            shmdt(sh_mem);
            kill(pid,SIGTERM);
            raise(SIGTERM);
          }
        }
        if (shmdt(sh_mem) == -1)
        {
          fprintf(stderr, "shmdt failed\n");
          exit(EXIT_FAILURE);
        }
    }
    else if(pid == -1)
    {
       perror("Forking failed"); 
       exit(EXIT_FAILURE); 
    }
    else//child consumes
    {
      while (running)
      {
        if (sh_area->written && sh_area->turn==2)//if theres data in sm
        {
          printf("%s", sh_area-> data);
          sh_area->written = 0;
          sh_area->turn = 0;
          
        if (strncmp(sh_area->data, "end chat", 8) == 0)
          {
            running = 0;
            shmdt(sh_mem);
            kill(getppid(),SIGTERM);
            raise(SIGTERM);
          }
        }
          sleep(rand() % 2);
        }
    }
  }
  else if (strcmp(*argv, "2") == 0)
  {
    pid_t pid;
    pid = fork();
    sh_mem = shmat(shmid, NULL, 0);
    sh_area = (struct shm_st *) sh_mem;

    
    if(pid>0)//parent produces
    {
      signal(SIGTERM,signal_handler);
      while (running)
      {
        while (sh_area->turn == 1 && sh_area->written == 0)
        {
          sleep(1); 
          //printf("Waiting...\n");
        }
        fgets(buffer, BUFSIZ, stdin);
        strcpy(sh_area->data, buffer);
        sh_area->written = 1;
        sh_area->turn = 2;
        if (strncmp(buffer, "end chat", 8) == 0){
            running = 0;
            shmdt(sh_mem);
            kill(pid,SIGTERM);
            raise(SIGTERM);
          }
        }
        if (shmdt(sh_mem) == -1)
        {
          fprintf(stderr, "shmdt failed\n");
          exit(EXIT_FAILURE);
        }
    }
    else if(pid == -1)
    {
       perror("Forking failed"); 
       exit(EXIT_FAILURE); 
    }
    else//child consumes
    {
      while (running)
      {
        if (sh_area->written==1 && sh_area->turn==1)//if theres data in sm
        {
          printf("%s", sh_area-> data);
          sh_area->written = 0;
          sh_area->turn=0;
          if (strncmp(sh_area->data, "end chat", 8) == 0)
          {
            running = 0;
            shmdt(sh_mem);
            kill(getppid(),SIGTERM);
            raise(SIGTERM);
          }
        }
          sleep(rand() % 2);
        }
    }
  }
  exit(EXIT_SUCCESS);
}