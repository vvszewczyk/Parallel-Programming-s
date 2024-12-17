#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t mutex;
static pthread_cond_t condition;

static int counter;
static int p;

void bariera_init(int threads)
{
    counter = 0;
    p = threads;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
}

void bariera(void)
{
    pthread_mutex_lock(&mutex);
    counter++;

    if(counter != p)
    {
        pthread_cond_wait(&condition, &mutex);
    }
    else
    {
        counter = 0;
        pthread_cond_broadcast(&condition);
    }

    pthread_mutex_unlock(&mutex);
}