#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* Prints Xs to stderr. */
void *print_Xs(void *unused)
{
  int n;
for (n = 100; n > 0; n--)
{
  fputc ('X', stderr); usleep(1);
}
return NULL;
}

  /* Prints Ys to stderr. */
  void* print_Ys(void* unused)//points to anything
  {
  int n;
  for (n = 100; n > 0; n--)
  {
    fputc ('Y', stderr); usleep(1);
  }
  return NULL;
  }

//main thread, thread 1, thread2 
//use main thread to wait (by using pthread_join)for thread1,thread2 to finish
/* The main program. */
int main()
{
  int n;
  pthread_t thread_id1, thread_id2;
  /* Create a new thread. The new thread will run the print_xs function. */
  pthread_create (&thread_id1, NULL, &print_Xs, NULL);
  /* Create a new thread. The new thread will run the print_ys function. */
  pthread_create (&thread_id2, NULL, &print_Ys, NULL);
  /* Print Zs to stderr. */
  for (n = 100; n > 0; n--)
  {
    fputc('Z', stderr); usleep(1);
  }

  pthread_join(thread_id1, NULL); //thread ID and return value
  pthread_join(thread_id2, NULL);
return 0;
}
  
//output is different because these threads have the same priority (same runtime), so the scheduler can switch the priority of the threads each time
//main thread finishes first = end of the whole process 