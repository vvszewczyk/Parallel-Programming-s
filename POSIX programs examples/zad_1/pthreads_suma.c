#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include"pomiar_czasu.h"

#define N 1000
#define p 4

pthread_mutex_t muteks; 
pthread_t watki[p];
double global_array_of_local_sums[p];
double *tab;
double suma=0; 

double suma_sekwencyjna(double *tab, int rozmiar) 
{
    double suma = 0.0;
    for (int i = 0; i < rozmiar; i++) 
    {
        suma += tab[i];
    }
    return suma;
}

void *suma_w( void *arg_wsk)
{
  int moj_id = *((int *)arg_wsk);

  int el_na_watek = ceil((float)N / p);
  int moj_start = el_na_watek * moj_id; // moj_id
  int moj_koniec = el_na_watek * (moj_id + 1); // N
  int moj_skok = 1; // p
  double moja_suma = 0; // redukcja

  if (moj_koniec > N) moj_koniec = N; // ostatni wątek kończy się na końcu przedziału N (obsługa niepodzielności N / p)

  for(int i = moj_start; i < moj_koniec; i += moj_skok)
  {
    moja_suma += tab[i]; // można zastąpić i
  }

    pthread_mutex_lock(&muteks);
    suma += moja_suma;
    pthread_mutex_unlock(&muteks);

  pthread_exit(NULL);

}

void *suma_w_no_mutex( void *arg_wsk)
{
    int moj_id = *((int *)arg_wsk);
    int el_na_watek = ceil((float)N / p);
    int moj_start = el_na_watek * moj_id; // moj_id
    int moj_koniec = el_na_watek * (moj_id + 1); // N
    int moj_skok = 1; // p
    double moja_suma = 0;

    if (moj_koniec > N) moj_koniec = N; // ostatni wątek kończy się na końcu przedziału N (obsługa niepodzielności N / p)

    for(int i = moj_start; i < moj_koniec; i += moj_skok)
    {
      moja_suma += tab[i]; // można zastąpić i
    }

    global_array_of_local_sums[moj_id] = moja_suma;

    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
  int i; 
  double t;
  int indeksy[p]; 

  for(i = 0; i < p; i++)
  {
    indeksy[i] = i;
  }


  tab = (double *) malloc(N * sizeof(double));
  for(i = 0; i < N; i++) 
  {
    tab[i] = ((double) i+1) / N; 
  }

  pthread_mutex_init(&muteks, NULL);

  // Wariant 1: Suma sekwencyjna
    t = czas_zegara();
    double suma_sekw_small = suma_sekwencyjna(tab, N);
    t = czas_zegara() - t;
    printf("Suma sekwencyjna: %lf, czas: %lf\n", suma_sekw_small, t);

  // Wariant 2: sumowanie z mutexekm
  suma = 0;
  t = czas_zegara();
  for(i = 0; i < p; i++) 
  {
    pthread_create(&watki[i], NULL, suma_w, (void *) &indeksy[i]);
  }

  for(i = 0; i < p; i++) 
  {
    pthread_join(watki[i], NULL);
  }
  t = czas_zegara() - t;

  printf("Suma z mutexem = %lf ", suma);
  printf("Czas obliczen %d wątków = %lf\n", p, t);

  // Wariant 3: sumowanie tablicy (bez mutexa)
  suma = 0;
  t = czas_zegara();
  for(i = 0; i < p; i++)
  {
    global_array_of_local_sums[i] = 0.0;
    pthread_create(&watki[i], NULL, suma_w_no_mutex, (void *) &indeksy[i]);
  }

  for(i = 0; i < p; i++)
  {
    pthread_join(watki[i], NULL);
    suma += global_array_of_local_sums[i];
  }
  t = czas_zegara() - t;
  printf("Wynik sumowania (za pomocą globalnej tablicy) bez mutexa: %lf ", suma);
  printf("Czas obliczen %d wątków = %lf\n", p, t);

  pthread_mutex_destroy(&muteks);
  free(tab);

  return 0;
}