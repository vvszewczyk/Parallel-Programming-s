#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include <stdlib.h>

typedef struct
{
  int id;
  double a;
  double b;
  double dx;
  //double res;
} Data2;

double funkcja (double x);

void* calka_podobszar_w(void* arg_wsk)
{
  Data2 *data = (Data2*)arg_wsk;
  int my_id = data->id;
  double a_local = data->a;
  double b_local = data->b;
  double dx = data->dx;

  //printf("\nWątek %d: a_local %lf, b_local %lf, dx %.8lf\n", my_id, a_local, b_local, dx);

  // wywołanie zadania do wykonania: całkowanie w zadanym przedziale 
  // calka = calka_sekw(a_local, b_local, dx);
  // (trzeba także dodać prototyp na początku pliku)
  
  int N_local = ceil((b_local - a_local) / dx);
  double dx_adjust_local = (b_local - a_local) / N_local;
  //printf("a_local %lf, b_local %lf, dx_adjust_local %.8f, n_local %d\n", a_local, b_local, dx_adjust_local, N_local);

  //double calka = 0.0;
  double *calka = (double*)malloc(sizeof(double));
  *calka = 0.0;
  for(int i = 0; i < N_local; i++)
  {
    double x1 = a_local + i * dx_adjust_local;
    //calka += 0.5 * dx_adjust_local * (funkcja(x1) + funkcja(x1 + dx_adjust_local));
    *calka += 0.5 * dx_adjust_local * (funkcja(x1) + funkcja(x1 + dx_adjust_local));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", i, x1, funkcja(x1), calka);
  }

  //data->res = calka;
  //pthread_exit(NULL);
  pthread_exit((void*) calka);
}

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w)
{

  //printf("a %lf, b %lf, dx %.8lf\n", a, b, dx);
  pthread_t threads[l_w];
  Data2 threadData2[l_w];
  double calka_suma_local = 0.0;

  for(int i = 0; i < l_w; i++)
  {
    threadData2[i].id = i;
    threadData2[i].a = a + ((b - a) / l_w) * i;
    threadData2[i].b = a + ((b - a) / l_w) * (i + 1);
    threadData2[i].dx = dx;
    //threadData2[i].res = 0.0;
  }

  for(int i = 0; i < l_w; i++)
  {
    pthread_create(&threads[i], NULL, calka_podobszar_w, &threadData2[i]);
  }

  for(int i = 0; i < l_w; i++)
  {
    double *res;
    //pthread_join(threads[i], NULL);
    //calka_suma_local += threadData2[i].res;
    pthread_join(threads[i], (void**)&res);
    calka_suma_local += *res;
    free(res);
  }

  return(calka_suma_local);
}
