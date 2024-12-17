#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct 
{
    int writers;
    int readers;
    int waitingWriters;
    int waitingReaders;
    int forceWriter;
} RR;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(RR* readingRoom);
void czytam(RR* readingRoom);
void pisze(RR* readingRoom);

int my_read_lock_lock(RR* readingRoom);
void my_read_lock_unlock(RR* readingRoom);
int my_write_lock_lock(RR* readingRoom);
void my_write_lock_unlock(RR* readingRoom);
void debugging(RR* obj);

#endif
