#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#define p 4

void *threadFunction(void * arg)
{
    int localID = *((int*)arg);
    pthread_t pid = pthread_self();

       printf("Thread number: %d, system ID: %ld\n", localID, pid);     

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[p];
    int pids[p];

    for(int i = 0; i < p; i++)
    {
        pids[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, &pids[i]/*&i*/);
        //sleep(1);
    }

    for(int i = 0; i < p; i++)
    {
        pthread_join(threads[i], NULL); 
    }

    pthread_exit(NULL);
}