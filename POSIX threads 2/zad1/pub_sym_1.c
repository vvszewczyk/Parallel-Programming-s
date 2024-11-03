#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

int availableMugs; 

void * watek_klient (void * arg_wsk)
{
  int moj_id = * ((int *)arg_wsk);
  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(int i = 0; i < ile_musze_wypic; i++)
  {
    // Aktywne oczekiwanie, aż kufel będzie dostępny
    while (1) 
    {
      if (availableMugs > 0)
      {
        availableMugs--;
        printf("\nKlient: %d pobieram kufel. Dostępnych kufli: %d\n", moj_id, availableMugs);
        break;
      }
      printf("\nKlient %d, brak wolnych kufli. Czekam...\n", moj_id);
      usleep(100000);
    }

    printf("\nKlient %d, nalewam z kranu\n", moj_id); 
    usleep(30000); // Symulacja nalewania
    printf("\nKlient %d, pije\n", moj_id); 
    nanosleep((struct timespec[]){{0, 50000000L}}, NULL);

    availableMugs++;
    printf("\nKlient %d, odkladam kufel. Dostępnych kufli: %d\n", moj_id, availableMugs); 
  }

  printf("\nKlient %d, wychodzę z pubu\n", moj_id); 
    
  return(NULL);
} 

int main(void)
{
  pthread_t *tab_klient;
  int *tab_klient_id;
  int startMugs;

  // liczba klientow
  int l_kl; 

  // liczba kufli (1L)
  int l_kf; 

  // liczba kranów
  int l_kr;


  printf("\nLiczba klientow: "); 
  scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); 
  scanf("%d", &l_kf);

  availableMugs = l_kf;
  startMugs = l_kf;



  //printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  l_kr = 1000000000; // wystarczająco dużo, żeby nie było rywalizacji 

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));

  for(size_t i = 0;i < l_kl; i++) 
  {
    tab_klient_id[i] = i;
  }

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 

  for(size_t i = 0;i < l_kl; i++)
  {
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 
  }

  for(size_t i = 0;i < l_kl; i++)
  {
    pthread_join(tab_klient[i], NULL);
  }

  if(availableMugs == startMugs)
  {
    printf("Sprawdzanie liczby kufli: %d\n", availableMugs);
  }
  else
  {
    printf("Liczba kufli uległa zmianie: %d\n", availableMugs);
  }

  printf("\nZamykamy pub!\n");


}