#include <pthread.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void *thread_f_0(void *arg)
{
  int n=0;
  for(n=0; n<100; n++)
  {
    printf("0");
    usleep(rand() % 1000);
  }
}

void *thread_f_1(void *arg)
{
  int n=0;
  for(n=0; n<100; n++)
  {
    printf("1");
    usleep(rand() % 1000);
  }
}

int main(int argc, char *argv[])
{
  pthread_t thread_0;
  pthread_t thread_1;
  
  pthread_create(&thread_0, NULL, thread_f_0, NULL);
  pthread_create(&thread_1, NULL, thread_f_1, NULL);

  pthread_join(thread_0, NULL);
  pthread_join(thread_1, NULL);
  
  pthread_detach(thread_0);
  pthread_detach(thread_1);
  
  printf("\n exit()\n");
  
  return 0;
}
