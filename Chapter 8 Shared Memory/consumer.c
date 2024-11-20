#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MEM_SIZE 4096
//consumer : waiting for the data from shar4ed memory
struct shm_st 
{
  int written1;
  char data1[BUFSIZ];
  int written2;
  char data2[BUFSIZ];
};
int main()
{
  int running = 1, shmID;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  srand((unsigned int) getpid()); //ensures that the random num is different. getpid has a unique id for sure. 
  int shmid = shmget((key_t) 1234, MEM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
      fprintf(stderr, "shmget failed\n");
      exit(EXIT_FAILURE);
    }
    sh_mem = shmat(shmID, NULL, 0); //NULL means let the system decide the location of the heap
    if (sh_mem == (void *) -1)
    {
      fprintf(stderr, "shmat failed\n");
      exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n", sh_mem); //print the location
    sh_area = (struct shm_st *) sh_mem; //type casting to access the element inside the structure
    sh_area->written = 0;
    while (running)
    {
      if (sh_area->written) //if its 1, it has a new data
      {
      printf("Data written in shared memory: %s\n", sh_area-> data);
      sh_area->written = 0;//means that location is ready for a new data (data has been read, so its waiting for a new one)
      if (strncmp(sh_area->data, "end", 3) == 0)
      running = 0;
      }
      sleep(rand() % 4);
    }
      if (shmdt(sh_mem) == -1 || shmctl(shmid, IPC_RMID, 0) == -1)//detach from the process and destroy the shared memory
    {
      fprintf(stderr, "shmdt or shmctl failed\n");
      exit(EXIT_FAILURE);
    }
      exit(EXIT_SUCCESS);
  }