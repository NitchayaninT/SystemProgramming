//Pitchapa Phisutpichet 6580065
//Nitchayanin Thamkunanon 6580081
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 5
#define MAX_PRIMES 10000

int storePrimes[THREADS][MAX_PRIMES];
int amountofPrimes[THREADS];//collect amount of primes from each thread

struct ThreadArgs{ //struct for each thread
  int start;
  int threadno;
  int count;//count how many primes in each thread
};

void* compute_prime (void* arg)//pass start, threadno, count (struct)
{
  struct ThreadArgs* args = (struct ThreadArgs*) arg;
  int candidate = 1 + args->start;
  int count = args->count;
  
  while (1)
  {
    if(candidate == (args->start)+9999)
    {
      args->count = count;
      return (void*) args;//return struct
    }
    int factor, is_prime = 1;
    
    for (factor = 2; factor < candidate; ++factor)
      if (candidate % factor == 0)
      { 
        is_prime = 0; //not prime
        break;
      }
      if (is_prime)
      {
        storePrimes[args->threadno][count]=candidate;
        count++;
      }
    ++candidate;
  }
return NULL;
}

void print()
{
  //print all primes
  for(int i=0;i<THREADS;i++)
    {
      printf("--THREAD %d--",i+1);
      printf("\n");
      for(int j=0;j<amountofPrimes[i];j++)
        {
          printf("%d ",storePrimes[i][j]);
        }

      printf("\n");
    }
}
//MAIN
//allow 5 threads to concurrently search the prime number between 1-50000
//each thread analyzes 10000 numbers
int main ()
  {
    pthread_t threads[5];
    struct ThreadArgs thread_args[5];

    for(int i=0; i<THREADS; i++)
    {
        thread_args[i].threadno = i;
        thread_args[i].start = 1 + (i*10000);
        thread_args[i].count = 0;
    }

    for(int i=0; i<THREADS; i++) //create
    {
        pthread_create(&threads[i], NULL, &compute_prime, &thread_args[i]);
    }
 
    for(int i=0; i<THREADS; i++) //joining
    {
        pthread_join(threads[i], (void*)&thread_args[i]);
    }

    for(int i=0;i<THREADS;i++)
    {
        amountofPrimes[i]=thread_args[i].count;
    }
 
    print();

  return 0;
  }
//runtime of a multithread program is slower than single threaded because since these threads run concurrently, 
//the OS has to switch between threads which can add significant overhead. Also, creating and managing new threads consume a lot of time.
