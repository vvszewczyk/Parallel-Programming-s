#include<stdio.h>
#include<math.h>
#include"pomiar_czasu.h"

#ifndef M_PI // standardy C99 i C11 nie wymagają definiowania stałej M_PI
#define M_PI (3.14159265358979323846)
#endif

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w, char mode);
double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w);
double funkcja (double x);

double calka_sekw(double a, double b, double dx)
{

  int N = ceil((b - a) / dx);
  double dx_adjust = (b - a) / N;
  int i;
  double calka = 0.0;
  //printf("Obliczona liczba trapezów: N = %d, dx_adjust = %lf\n", N, dx_adjust);
  //printf("a %lf, b %lf, N %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);
  
  for(i = 0; i < N; i++)
  {
    double x1 = a + i * dx_adjust;
    calka += 0.5 * dx_adjust * (funkcja(x1) + funkcja(x1 + dx_adjust));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", i, x1, funkcja(x1), calka);
  }

  return(calka);
}


int main(int argc, char *argv[])
{
  double a = 0.0;
  double b = M_PI;
  double t;

  double dxs[] = {0.1, 0.01, 0.0001, 0.00001, 0.0000001};
  int amount = sizeof(dxs) / sizeof(dxs[0]);

  printf("\nProgram obliczania całki z funkcji (sinus) metodą trapezów.\n");

  // =================== Sekwencyjne ===================
  for(int i = 0; i < amount; i++)
  {
    double dx = dxs[i];
    printf("\nPoczatek obliczeń sekwencyjnych\n");

    t = czas_zegara();
    double calka = calka_sekw(a, b, dx);
    t = czas_zegara() - t;

    printf("Koniec obliczen sekwencyjnych\n");
    printf("\ndx: %.8f:\n", dx);
    printf("Całka: %.15lf\n", calka);
    printf("Błąd: %.15lf\n", 2.0 - calka);
    printf("Czas wykonania: %lf sekund\n", t);
  }
  
  // ==================== Równoległe ======================
  double dx = dxs[4];
  double l_w = 4;
  // c - cykliczny, b - blokowy
  char mode = 'b';
  printf("\nPoczatek obliczeń równoległych (zrównoleglenie pętli)\n");
  
  t = czas_zegara();
  double calka = calka_zrownoleglenie_petli(a, b, dx, l_w, mode);
  t = czas_zegara() - t;

  //printf("\nKoniec obliczen  równoległych (zrównoleglenie pętli) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t, calka);


// ====================== Dekompozycja obszaru ========================
  printf("\nPoczatek obliczeń równoległych (dekompozycja obszaru)\n");

  t = czas_zegara();
  calka = calka_dekompozycja_obszaru(a, b, dx, l_w);
  t = czas_zegara() - t;

  printf("\nKoniec obliczen  równoległych (dekompozycja obszaru) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t, calka);

  return 0;
}