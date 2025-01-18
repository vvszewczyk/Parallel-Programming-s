#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "sortowanie_seq.h"

// parametr wykorzystywany do ograniczania liczby tworzonych zadań i wątków
#define max_poziom 3

void merge_sort_openmp(double *A, int p, int r, int poziom)
{

    /*++++++++++++++++ executable statements ++++++++++++++++*/

    if (p < r)
    {

        // if(poziom<5) printf("watek %d, poziom %d\n", omp_get_thread_num(),poziom);

        poziom++;

        int q1 = (p + r) / 2;

#pragma omp task final(poziom > max_poziom) default(none) firstprivate(A, p, r, q1, poziom)
        {
            merge_sort_openmp(A, p, q1, poziom);
        }

#pragma omp task final(poziom > max_poziom) default(none) firstprivate(A, p, r, q1, poziom)
        {
            merge_sort_openmp(A, q1 + 1, r, poziom);
        }

#pragma omp taskwait

        scal(A, p, q1, r);
    }

    return;
}

void merge_sort_openmp_2(double *A, int p, int r, int poziom)
{

    /*++++++++++++++++ executable statements ++++++++++++++++*/

    if (p < r)
    {

        // if(poziom < 4)
        // {
        //   printf("watek %d, poziom %d\n", omp_get_thread_num(),poziom);
        // }

        poziom++;

        int q1 = (p + r) / 2;

// modyfikacja 1 - uzycie klauzuli final tak jak w merge_sort_openmp()
#pragma omp task final(poziom > max_poziom) default(none) firstprivate(A, p, r, q1, poziom)
        {
            // final(warunek) ogranicza tworzenie zadań gdy warunek jest spełniony, zadania tworzone są przez 1 wątek
            // taskwait - synchronizuje zadania
            // omp_in_final() - sprawdza czy aktualnie wykonywane zadanie zostało oznaczone jako finalne
            // task musi być wywoływany w obszarze równoległym (tu jest w main) i jeden wątek (single) rozdziela zadania

            // modyfikacja 2 - w przypadku kiedy obowiązuje warunek z final (sprawdzenie
            // funkcją omp_in_final() ) uruchomienie funkcji sortowania szybkiego
            // z biblioteki sortowanie_seq: sortowanie_szybkie(A,p,q1);
            if (omp_in_final())
            {
                sortowanie_szybkie(A, p, q1);
            }
            else
            {
                merge_sort_openmp_2(A, p, q1, poziom);
            }
        }

// modyfikacja 1 - uzycie klauzuli final tak jak w merge_sort_openmp()
#pragma omp task final(poziom > max_poziom) default(none) firstprivate(A, p, r, q1, poziom)
        {
            // modyfikacja 2 - w przypadku kiedy obowiązuje warunek z final (sprawdzenie
            // funkcją omp_in_final() ) uruchomienie funkcji sortowania szybkiego
            // z biblioteki sortowanie_seq: sortowanie_szybkie(A,q1+1,r);
            if (omp_in_final())
            {
                sortowanie_szybkie(A, q1 + 1, r);
            }
            else
            {
                merge_sort_openmp_2(A, q1 + 1, r, poziom);
            }
        }

#pragma omp taskwait

        scal(A, p, q1, r);
    }

    return;
}

void merge_sort_openmp_4(double *A, int p, int r, int poziom)
{

    /*++++++++++++++++ executable statements ++++++++++++++++*/

    poziom++;
    if (poziom > max_poziom)
    {
        sortowanie_szybkie(A, p, r);
    }
    else
    {
        if (p < r)
        {
            int q1 = (p + r) / 2;

// sections dzieli kod na niezależne sekcje które mogą być wykonywane równolegle, nie ma wyścigu, system używa wątków
// które są dostępne, na końcu jest bariera (chyba że damy nowait)
#pragma omp parallel sections default(none) firstprivate(A, p, r, q1, poziom)
            {
#pragma omp section
                {
                    merge_sort_openmp_4(A, p, q1, poziom);
                }

#pragma omp section
                {
                    merge_sort_openmp_4(A, q1 + 1, r, poziom);
                }
            }

            scal(A, p, q1, r);
        }
    }
    return;
}
