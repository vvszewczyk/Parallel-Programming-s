#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t readable = PTHREAD_COND_INITIALIZER;
static pthread_cond_t writable = PTHREAD_COND_INITIALIZER;

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(RR* readingRoom)
{
    pthread_mutex_lock(&mutex);

    readingRoom->waitingReaders++;

    while(readingRoom->writers > 0 || readingRoom->forceWriter)
    {
        pthread_cond_wait(&readable, &mutex);
    }

    readingRoom->waitingReaders--;
    readingRoom->readers++;

    debugging(readingRoom);

    pthread_mutex_unlock(&mutex);
    return 0;
}


void my_read_lock_unlock(RR* readingRoom)
{
    pthread_mutex_lock(&mutex);

    readingRoom->readers--;

    debugging(readingRoom);

    if(readingRoom->readers == 0)
    {
        pthread_cond_signal(&writable);
    }

    pthread_mutex_unlock(&mutex);
}


int my_write_lock_lock(RR* readingRoom)
{
    pthread_mutex_lock(&mutex);

    
    readingRoom->forceWriter = 1;

    while(readingRoom->readers > 0 || readingRoom->writers > 0)
    {
        readingRoom->waitingWriters++;
        pthread_cond_wait(&writable, &mutex);
        readingRoom->waitingWriters--;
    }

   
    readingRoom->writers++;

    debugging(readingRoom);

    pthread_mutex_unlock(&mutex);
    return 0;
}


void my_write_lock_unlock(RR* readingRoom)
{
    pthread_mutex_lock(&mutex);

    readingRoom->writers--;

    debugging(readingRoom);

    readingRoom->forceWriter = 0;

    if(readingRoom->waitingReaders > 0)
    {
        pthread_cond_broadcast(&readable);
    }
    else if(readingRoom->waitingWriters > 0)
    {
        pthread_cond_signal(&writable);
    }

    pthread_mutex_unlock(&mutex);
}

void inicjuj(RR* readingRoom)
{
    readingRoom->writers = 0;
    readingRoom->readers = 0;
    readingRoom->waitingWriters = 0;
    readingRoom->waitingReaders = 0;
    readingRoom->forceWriter = 0;
}

void debugging(RR* obj)
{
    #ifdef MY_DEBUG

    int writer_count = obj->writers;
    int reader_count = obj->readers;

    printf("Writers: %d\n Readers: %d\n", writer_count, reader_count); 
    if(writer_count > 1) 
    {
        printf("### Error: Simultaneous presence of more than one writer.\n");
        exit(0);
    } 
    else if(writer_count == 1 && reader_count > 0) 
    {
        printf("### Error: Error: Simultaneous presence of writer and readers.\n");
        exit(0);
    } 
    else if(writer_count < 0) 
    {
        printf("### Error: Number of writers is negative.\n");
        exit(0);
    } 
    else if(reader_count < 0) 
    {
        printf("### Error: Number of readers is negative.\n");
        exit(0);
    }  

    #endif
}

void czytam(RR* readingRoom)
{
    debugging(readingRoom);
    usleep(rand()%3000000);
}

void pisze(RR* readingRoom)
{
    debugging(readingRoom);
    usleep(rand()%3000000);
}
