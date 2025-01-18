#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void mat_vec_row(double *a, double *x, double *y, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        y[i] = 0.0;
        for (j = 0; j < n; j++)
        {
            y[i] += a[n * i + j] * x[j];
        }
    }
}

// Dekompozycja wierszowa, przechowywanie macierzy wierszami
void mat_vec_row_row_decomp(double *a, double *x, double *y, int n)
{
#pragma omp parallel for default(none) shared(n, y, a, x)
    for (int i = 0; i < n; i++)
    {
        y[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            y[i] += a[n * i + j] * x[j];
        }
    }
}

// Dekompozycja kolumnowa, przechowywanie macierzy wierszami
void mat_vec_row_col_decomp(double *a, double *x, double *y, int n)
{
#pragma omp parallel for default(none) shared(n, a, x) reduction(+ : y[ : n]) // lokalna kopia tablicy
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
        {
            y[i] += a[n * i + j] * x[j];
        }
    }
}

void mat_vec_col(double *a, double *x, double *y, int n)
{

    int i, j;

    for (i = 0; i < n; i++)
    {
        y[i] = 0.0;
    }
    for (j = 0; j < n; j++)
    {
        for (i = 0; i < n; i++)
        {
            y[i] += a[i + j * n] * x[j];
        }
    }
}

// Dekompozycja kolumnowa dla macierzy przechowywanych kolumnami
void mat_vec_col_col_decomp(double *a, double *x, double *y, int n)
{
    for (int i = 0; i < n; i++)
    {
        y[i] = 0.0;
    }

#pragma omp parallel shared(n, y, a, x)
    {
        double *y_local = malloc(n * sizeof(double)); // prywatny wektor dla kazdego wątku

        for (int i = 0; i < n; i++)
        {
            y_local[i] = 0.0;
        }

#pragma omp for
        for (int j = 0; j < n; j++)
        {
            for (int i = 0; i < n; i++)
            {
                y_local[i] += a[i + j * n] * x[j];
            }
        }

#pragma omp critical
        {
            for (int i = 0; i < n; i++)
            {
                y[i] += y_local[i];
            }
        }
    }
}

// Dekompozycja wierszowa dla macierzy przechowywanych kolumnami
void mat_vec_col_row_decomp(double *a, double *x, double *y, int n)
{
#pragma omp parallel for default(none) shared(n, y, a, x)
    for (int i = 0; i < n; i++)
    {
        y[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            y[i] += a[i + j * n] * x[j];
        }
    }
}
