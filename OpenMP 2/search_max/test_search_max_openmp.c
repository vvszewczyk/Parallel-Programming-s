#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "search_max_openmp.h"

#define WYMIAR 1e9
#define ROZMIAR WYMIAR

int main()
{
    double *a = malloc(ROZMIAR * sizeof(double));
    int n, i, j;
    double a_max;
    double t1;

    for (i = 0; i < ROZMIAR; i++)
        a[i] = (0.00000001 * i) * (ROZMIAR - 1 - i);

    n = ROZMIAR;

    t1 = omp_get_wtime();
    a_max = search_max(a, 0, n - 1);
    t1 = omp_get_wtime() - t1;
    printf("search_max\nmaximal element %lf\n", a_max);
    printf("time for sequential linear search: %lf\n\n", t1);

    t1 = omp_get_wtime();
    a_max = search_max_openmp_simple(a, 0, n - 1);
    t1 = omp_get_wtime() - t1;
    printf("search_max_openmp_simple\nmaximal element %lf\n", a_max);
    printf("time for parallel linear search: %lf\n\n", t1);

    t1 = omp_get_wtime();
    a_max = search_max_openmp_task(a, 0, n - 1);
    t1 = omp_get_wtime() - t1;
    printf("search_max_openmp_task\nmaximal element %lf\n", a_max);
    printf("time for parallel linear search (tasks): %lf\n\n", t1);

    t1 = omp_get_wtime();
    a_max = bin_search_max(a, 0, n - 1);
    t1 = omp_get_wtime() - t1;
    printf("bin_search_max\nmaximal element %lf\n", a_max);
    printf("time for sequential binary search: %lf\n\n", t1);

    t1 = omp_get_wtime();
    a_max = bin_search_max_openmp(a, 0, n - 1);
    t1 = omp_get_wtime() - t1;
    printf("bin_search_max_openmp\nmaximal element %lf\n", a_max);
    printf("time for parallel binary search: %lf\n\n", t1);

    free(a);
}
