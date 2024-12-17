#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include"czytelnia.h"

void *funkcja_czytelnika(void * arg)
{
  RR* cz_p = (RR *)arg;
  
  for(;;)
  {
    usleep(rand()%1000000);
    printf("READER %lu - before lock\n", pthread_self());
    
    my_read_lock_lock(cz_p);
    
    printf("READER %lu - go in\n", pthread_self());
    
    czytam(cz_p);
    
    
    printf("READER %lu - leave\n", pthread_self());
    
    my_read_lock_unlock(cz_p);
    
    printf("READER %lu - after lock\n", pthread_self());
    
  }
}

void *funkcja_pisarza( void * arg)
{
  RR* cz_p = (RR *)arg;
  
  for(;;)
  {
    usleep(rand()%3000000);
    printf("===================WRITER %lu - before lock\n", pthread_self());
    
    my_write_lock_lock(cz_p);
    
    printf("===================WRITER %lu - go in\n", pthread_self());
    
    pisze(cz_p);
    
    printf("==================WRITER %lu - leave\n", pthread_self());
    
    my_write_lock_unlock(cz_p);
    
    printf("==================WRITER %lu - after lock\n", pthread_self());
  }
}


int main()
{
  pthread_t pisarze[5], czytelnicy[10];
  int indeksy[10] = {0,1,2,3,4,5,6,7,8,9}; 
  RR czytelnia;
  
  inicjuj(&czytelnia);
    
  for(int i = 0; i < 5; i++)
  {
    pthread_create(&pisarze[i], NULL, funkcja_pisarza, (void *)&czytelnia);
  }

  for(int i = 0; i < 10; i++)
  {
    pthread_create(&czytelnicy[i], NULL, funkcja_czytelnika, (void *)&czytelnia);
  }

  for(int i = 0; i < 5; i++)
  {
    pthread_join(pisarze[i], NULL); 
  }

  for(int i = 0; i < 10; i++)
  {
    pthread_join(czytelnicy[i], NULL); 
  }
  
}
