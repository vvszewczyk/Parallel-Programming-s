#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

struct struktura_t 
{ 
  int l_wkf; // Liczba kufli
  pthread_mutex_t *tab_kuf; // Tablica mutexow kufli
  int l_kr; // Liczba kranow
  pthread_mutex_t *tab_kran;  // Tablica mutexow kranow
};

struct struktura_t pub_wsk;

void *watek_klient (void *arg_wsk)
{
  int moj_id = * ((int *)arg_wsk);

  int kran, kufel, success;

  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(int i_wypite = 0; i_wypite < ile_musze_wypic; i_wypite++)
  {
    kufel = ((moj_id + 1) * (i_wypite + 2)) % pub_wsk.l_wkf;
    pthread_mutex_lock(&pub_wsk.tab_kuf[kufel]);
    printf("\nKlient %d, wybieram kufel %d\n", moj_id, kufel); 

    success = 0;
    do
    {
      kran = rand() % pub_wsk.l_kr;
      printf("\nKlient %d, wybieram kran %d\n", moj_id, kran); 

      if(pthread_mutex_trylock(&pub_wsk.tab_kran[kran]) == 0)
      {
        printf("\nKlient %d, nalewam z kranu %d\n", moj_id, kran); 
        usleep(100);

        if(kran == 0)  
        {
          printf("\nKlient %d, pije piwo Guinness\n", moj_id); 
        }
        else if(kran == 1)  
        {
          printf("\nKlient %d, pije piwo Żywiec\n", moj_id); 
        }
        else if(kran == 2)  
        {
          printf("\nKlient %d, pije piwo Heineken\n", moj_id); 
        }
        else if(kran == 3)  
        {
          printf("\nKlient %d, pije piwo Okocim\n", moj_id); 
        }
        else if(kran == 4)  
        {
          printf("\nKlient %d, pije piwo Carlsberg\n", moj_id); 
        }
        else
        {
          printf("\nKlient %d, pije piwo Żubr\n", moj_id);
        }

        pthread_mutex_unlock(&pub_wsk.tab_kran[kran]);
        success = 1;
      }
      else
      {
        printf("\nKlient %d, kran %d zajęty, czekam...\n", moj_id, kran);
        usleep(50);
      }
    }
    while(success == 0);

    usleep(100);

    pthread_mutex_unlock(&pub_wsk.tab_kuf[kufel]);
    printf("\nKlient %d, odkladam kufel %d\n", moj_id, kufel); 
  }

  return(NULL);
}

int main(void)
{
  pthread_t *tab_klient;
  int *tab_klient_id;
  int l_kl, l_kf, l_kr, i;

  printf("\nLiczba klientow: "); 
  scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); 
  scanf("%d", &l_kf);
  pub_wsk.l_wkf = l_kf;

  printf("\nLiczba kranow: "); 
  scanf("%d", &l_kr);
  pub_wsk.l_kr = l_kr;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));

  for(i = 0; i < l_kl; i++) 
  {
    tab_klient_id[i] = i;
  }

  pub_wsk.tab_kuf = (pthread_mutex_t *) malloc(l_kf*sizeof(pthread_mutex_t));
  for(i = 0; i < l_kf; i++) 
  {
    pthread_mutex_init(&pub_wsk.tab_kuf[i], NULL);
  }

  pub_wsk.tab_kran = (pthread_mutex_t *) malloc(l_kr*sizeof(pthread_mutex_t));
  for(i = 0; i < l_kr; i++) 
  {
    pthread_mutex_init(&pub_wsk.tab_kran[i], NULL);
  }

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 

  for(i = 0; i < l_kl; i++)
  {
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 
  }

  for(i = 0; i < l_kl; i++)
  {
    pthread_join(tab_klient[i], NULL);
  }

  for (i = 0; i < l_kf; i++) 
  {
    pthread_mutex_destroy(&pub_wsk.tab_kuf[i]);
  }

  for (i = 0; i < l_kr; i++) 
  {
    pthread_mutex_destroy(&pub_wsk.tab_kran[i]);
  }

  free(pub_wsk.tab_kuf);
  free(pub_wsk.tab_kran);
  free(tab_klient);
  free(tab_klient_id);

  printf("\nZamykamy pub!\n");
}
