#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include"pomiar_czasu.h"

const int liczba = 15000; // Na potrzeby zwiększenia dokładności liczbę zwiększono do 15000 z 1000

int main() 
{
    double a;
    int i, k;
    double t1 = 0.0, t2 = 0.0;
    k = -50000;

    // Pomiar czasu dla operacji wejścia/wyjścia
    inicjuj_czas();
    t1 = czas_zegara(); 
    t2 = czas_CPU();
    for(i = 0; i < liczba; i++) 
    {
        printf("%d ", k + i);
    }
    printf("\n");
    printf("Czas wykonania %d operacji wejscia/wyjscia: time Clock: %lf, CPU time: %lf\n", liczba, czas_zegara() - t1, czas_CPU() - t2);

    // Pomiar czasu dla operacji arytmetycznych
    a = 1.000001;
    t1 = czas_zegara(); 
    t2 = czas_CPU();
    for(i = 0; i < liczba; i++) 
    {
        a = 1.000001 * a + 0.000001; 
    }
    t1 = czas_zegara() - t1; 
    t2 = czas_CPU() - t2;

    printf("Wynik operacji arytmetycznych: %lf\n", a);
    printf("Czas wykonania %d operacji arytmetycznych: time Clock: %lf, CPU time: %lf\n", liczba, t1, t2);

    return 0;
}
