//Producer.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define SHM_NAME "/pshm"
//this is the file of shared memory "/pshm"
#define MEM_SIZE 64
struct shm_st
{
  int written;
  char data[MEM_SIZE];
};
int main()
{
int running = 1, shmfd, st_size;
void *sh_mem = NULL;
struct shm_st *sh_area;
  char buffer[BUFSIZ];
  st_size = sizeof(struct shm_st);
  shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660);
  if (shmfd == -1)
  {
    perror("shm_open failed ");
    exit(EXIT_FAILURE);
  }
  ftruncate(shmfd, st_size);
  sh_mem = mmap(0, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0); //map to the process
  printf("Memory attached at %p\n", sh_mem);
  sh_area = (struct shm_st *) sh_mem;
  sh_area->written = 0;

  while (running)
  {
    while (sh_area->written)
    {
      usleep(200);
    }
    printf("Enter data: ");
    fgets(buffer, BUFSIZ, stdin);
    strcpy(sh_area->data, buffer);
    sh_area->written = 1;
    if (strncmp(buffer, "end", 3) == 0)
      running = 0;
    }
    munmap(sh_mem, st_size);
    close(shmfd);
    exit(EXIT_SUCCESS);
    
  }