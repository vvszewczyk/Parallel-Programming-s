#include<stdio.h>
#include<pthread.h>
#include<math.h>

static int l_w_global = 0;
static double calka_global = 0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

pthread_mutex_t mutex;

typedef struct
{
  int my_id;
  double a, b;
  int l_w;
  char mode;
} Data;

double funkcja(double x){return sin(x);}

void* calka_fragment_petli_w(void* arg_wsk)
{
  Data *data = (Data *)arg_wsk;
  int my_id = data->my_id;
  double l_w = data->l_w;
  char mode = data->mode;

  double a = a_global;
  double dx = dx_global;
  int N = N_global;

  int my_start, my_end, my_stride;

  if(mode == 'c') // dekompozycja cykliczna
  {
    my_start = my_id;
    my_end = N;
    my_stride = l_w;
  }
  else if(mode == 'b') // dekompozycja blokowa
  {
    int el_on_thread = ceil((double)N / l_w);
    my_start = el_on_thread * my_id;
    my_end = el_on_thread * (my_id + 1); 
    if(my_end > N) my_end = N; // ostatni wątek kończy się na końcu przedziału N (obsługa niepodzielności N / l_w)
    my_stride = 1;
  }

  //printf("\nWątek %d: my_start %d, my_end %d, my_stride %d\n", my_id, my_start, my_end, my_stride);

  double calka = 0.0; // redukcja - wprowadzenie zmiennej lokalnej (operacje na zmiennych lokalnych są bezpieczne)
  for(int i = my_start; i < my_end; i += my_stride)
  {
    double x1 = a + i * dx;
    calka += 0.5 * dx * (funkcja(x1) + funkcja(x1 + dx));
  }

  pthread_mutex_lock(&mutex);
  calka_global += calka;
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);

  // something else ? (dekompozycja blokowo-cykliczna)
  
  //for(i=my_start; i<my_end; i+=my_stride){
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  //}
}

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w, char mode)
{
    pthread_t threads[l_w];
    Data threadData[l_w];
    a_global = a;
    b_global = b;
    dx_global = dx;
    N_global = ceil((b - a) / dx);
    l_w_global = l_w;
    calka_global = 0.0;
    double dx_adjust = (b - a) / N_global;

    //printf("Obliczona liczba trapezów: N = %d, dx_adjust = %.8lf\n", N_global, dx_adjust);
    //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < l_w; i++) 
    {
        threadData[i].my_id = i;
        threadData[i].l_w = l_w;
        threadData[i].mode = mode;
    }

    for (int i = 0; i < l_w; i++) 
    {
        pthread_create(&threads[i], NULL, calka_fragment_petli_w, &threadData[i]);
    }

    for (int i = 0; i < l_w; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

  return(calka_global);
}

