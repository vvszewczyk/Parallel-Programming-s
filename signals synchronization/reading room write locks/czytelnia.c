#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

/*** Implementacja procedur interfejsu ***/

//pthread_rwlock_rdlock: Pozwala wielu wątkom na jednoczesny dostęp w trybie czytania, o ile nie ma aktywnych pisarzy.
//pthread_rwlock_wrlock: Blokuje wszystkie wątki, zarówno czytelników, jak i innych pisarzy, dopóki pisarz nie skończy.
//pthread_rwlock_unlock: Zwalnia blokadę.

int my_read_lock_lock(cz_t* cz_p)
{
    pthread_rwlock_rdlock(&rwlock);
    pthread_mutex_lock(&mutex);

    cz_p->readers++; 
    debugging(cz_p);

    pthread_mutex_unlock(&mutex);
}

int my_read_lock_unlock(cz_t* cz_p)
{
    pthread_mutex_lock(&mutex);
    cz_p->readers--;
    debugging(cz_p);

    pthread_mutex_unlock(&mutex);
    pthread_rwlock_unlock(&rwlock);
    usleep(3000000);
    return 0;
}

int my_write_lock_lock(cz_t* cz_p)
{
    pthread_rwlock_wrlock(&rwlock);
    pthread_mutex_lock(&mutex);

    cz_p->writers++;

    debugging(cz_p);

    pthread_mutex_unlock(&mutex);

    return 0;
}

int my_write_lock_unlock(cz_t* cz_p)
{
    pthread_mutex_lock(&mutex);

    cz_p->writers--;

    debugging(cz_p);

    pthread_mutex_unlock(&mutex);
    pthread_rwlock_unlock(&rwlock);
}

void inicjuj(cz_t* cz_p)
{
  cz_p->writers = 0;
  cz_p->readers = 0;  
}

void debugging(cz_t* obj)
{
    #ifdef MY_DEBUG

    int writer_count = obj -> writers;
    int reader_count = obj -> readers;

    printf("Writers: %d\n Readers: %d\n", writer_count, reader_count); 
    if (writer_count > 1) 
    {
        printf("### Error: Simultaneous presence of more than one writer.\n");
        exit(0);
    } 
    else if (writer_count == 1 && reader_count > 0) 
    {
        printf("### Error: Error: Simultaneous presence of writer and readers.\n");
        exit(0);
    } 
    else if (writer_count < 0) 
    {
        printf("### Error: Number of writers is negative.\n");
        exit(0);
    } 
    else if (reader_count < 0) 
    {
        printf("### Error: Number of readers is negative.\n");
        exit(0);
    }  

    #endif
}

void czytam(cz_t* cz_p)
{
  debugging(cz_p);
  usleep(rand()%3000000);
}

void pisze(cz_t* cz_p)
{
  debugging(cz_p);
  usleep(rand()%3000000);
}


