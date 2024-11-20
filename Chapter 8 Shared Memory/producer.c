//Producer.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MEM_SIZE 4096
struct shm_st
{
  int written;
  char data[BUFSIZ];
};

int main()
{
int running = 1, shmID;
void *sh_mem = NULL;
struct shm_st *sh_area;
  char buffer[BUFSIZ];
  int shmid = shmget((key_t) 1234, MEM_SIZE,
  0666 | IPC_CREAT);
  if (shmid == -1)
  {
  fprintf(stderr, "shmget failed\n");
  exit(EXIT_FAILURE);
  }
  sh_mem = shmat(shmID, NULL, 0);
  if (sh_mem == (void *) -1)
  {
  fprintf(stderr, "shmat failed\n");
  exit(EXIT_FAILURE);
  }
  printf("Memory attached at %X\n", sh_mem);
  sh_area = (struct shm_st *) sh_mem;
    while (running) //wait for user to type something
    {
    while (sh_area->written)
    {
      sleep(1); 
      printf("Waiting...\n");
    }
    printf("Enter data: ");
    fgets(buffer, BUFSIZ, stdin); //get data if written = 0
    strcpy(sh_area->data, buffer);
    sh_area->written = 1;
    if (strncmp(buffer, "end", 3) == 0)
    running = 0;
    }
    if (shmdt(sh_mem) == -1)
    {
    fprintf(stderr, "shmdt failed\n");
    exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}