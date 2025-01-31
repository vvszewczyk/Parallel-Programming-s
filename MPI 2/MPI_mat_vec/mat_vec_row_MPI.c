#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"

// #define WYMIAR 6400
// #define ROZMIAR 40960000
// #define WYMIAR 7936
// #define ROZMIAR 62980096
// #define WYMIAR 19200
// #define ROZMIAR 368640000
// #define WYMIAR 40320 // divisible by 128
// #define WYMIAR 20160 // divisible by 64
#define WYMIAR 10080 // divisible by 1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,24,28,30,32,36,40,60
// #define WYMIAR 4800 // max possible for MPI_Alltoall on Estera
#define ROZMIAR (WYMIAR * WYMIAR)

// Slightly optimized version of matrix-vector product with possible OpenMP parallelization
void mat_vec(double *a, double *x, double *y, int nn, int nt)
{
    register int n = nn;
    register int i;
    // #pragma omp parallel for num_threads(nt) default(none) shared(a, x, y, n)
    for (i = 0; i < n; i += 2)
    {
        register double ty1 = 0;
        register double ty2 = 0;
        register int j;
        for (j = 0; j < n; j += 2)
        {
            register double t0 = x[j];
            register double t1 = x[j + 1];
            register int k = i * n + j;
            ty1 += a[k] * t0 + a[k + 1] * t1;
            ty2 += a[k + n] * t0 + a[k + 1 + n] * t1;
        }
        // printf("nr threads = %d\n", omp_get_num_threads());
        y[i] = ty1;
        y[i + 1] += ty2;
    }
}

int main(int argc, char **argv)
{
    static double x[WYMIAR], y[WYMIAR], z[WYMIAR]; // z is sized for column decomposition
    double *a;
    double t1;
    int n, nt, i, j;
    const int ione = 1;
    const double done = 1.0;
    const double dzero = 0.0;

    int rank, size, source, dest, tag = 0;
    int root = 1;
    int n_wier, n_wier_last;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n = WYMIAR;

    // x is locally initialized to zero
    for (i = 0; i < WYMIAR; i++)
    {
        x[i] = 0.0;
    }

    if (rank == root)
    {
        a = (double *)malloc((ROZMIAR + 1) * sizeof(double));

        for (i = 0; i < ROZMIAR; i++)
        {
            a[i] = 1.0 * i;
        }

        for (i = 0; i < WYMIAR; i++)
        {
            x[i] = 1.0 * (WYMIAR - i);
        }

        // printf("Podaj liczbe watkow: "); scanf("%d",&nt);
        nt = 1;

        printf("poczatek (wykonanie sekwencyjne)\n");

        t1 = MPI_Wtime();
        mat_vec(a, x, y, n, nt);
        t1 = MPI_Wtime() - t1;

        printf("\tczas wykonania (zaburzony przez MPI?): %lf, Gflop/s: %lf, GB/s> %lf\n", t1, 2.0e-9 * ROZMIAR / t1,
               (1.0 + 1.0 / n) * 8.0e-9 * ROZMIAR / t1);
    }

    if (size > 1)
    {

        /************** || block row decomposition || *******************/

        // z is initialized for all ranks
        for (i = 0; i < WYMIAR; i++)
        {
            z[i] = 0.0;
        }

        MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD);
        // podzial wierszowy
        n_wier = ceil(WYMIAR / size);
        n_wier_last = WYMIAR - n_wier * (size - 1);

        // for n_wier!=n_wier_last arrays should be oversized to avoid problems
        if (n_wier != n_wier_last)
        {
            printf("This version does not work with WYMIAR not a multiple of size!\n");
            MPI_Finalize();
            exit(0);
        }

        // local matrices a_local form parts of a big matrix a
        double *a_local = (double *)malloc(WYMIAR * n_wier * sizeof(double));
        for (i = 0; i < WYMIAR * n_wier; i++)
        {
            a_local[i] = 0.0;
        }

        // Rozproszenie równomiernych fragmentów tablicy a na dostępne procesy
        MPI_Scatter(a, WYMIAR * n_wier, MPI_DOUBLE, a_local, WYMIAR * n_wier, MPI_DOUBLE, root, MPI_COMM_WORLD);

        // Rozesłanie x z jednego procesu (root) do wszystkich pozostałych procesów w komunikatorze
        MPI_Bcast(x, WYMIAR, MPI_DOUBLE, root, MPI_COMM_WORLD);

        if (rank == root)
        {
            printf("Starting MPI matrix-vector product with block row decomposition!\n");
            t1 = MPI_Wtime();
        }

        // local parts of x start at rank*n_wier
        // MPI_Allgather(&x[rank * n_wier], n_wier, MPI_DOUBLE, x, n_wier, MPI_DOUBLE, MPI_COMM_WORLD);
        // MPI_Allgather(MPI_IN_PLACE, n_wier, MPI_DOUBLE, x, n_wier, MPI_DOUBLE, MPI_COMM_WORLD); // zbiera lokalnie
        // obliczone fragmenty danych (wektora x) i rozsyła je tak, aby każdy proces miał pełną informację

        for (i = 0; i < n_wier; i++)
        {

            double t = 0.0;
            int ni = n * i;

            for (j = 0; j < n; j++)
            {
                t += a_local[ni + j] * x[j];
            }
            // printf("rank %d: row %d, final y %lf\n", rank, i, t);
            z[i] = t;
        }

        // just to measure time
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == root)
        {
            t1 = MPI_Wtime() - t1;
            printf("Wersja rownolegla MPI z dekompozycją wierszową blokową\n");
            printf("\tczas wykonania: %lf, Gflop/s: %lf, GB/s> %lf\n", t1, 2.0e-9 * ROZMIAR / t1,
                   (1.0 + 1.0 / n) * 8.0e-9 * ROZMIAR / t1);
        }

        MPI_Gather(z, n_wier, MPI_DOUBLE, z, n_wier, MPI_DOUBLE, root, MPI_COMM_WORLD);

        if (rank == root)
        {
            for (i = 0; i < WYMIAR; i++)
            {
                if (fabs(y[i] - z[i]) > 1.e-9 * z[i])
                {
                    printf("Blad! i=%d, y[i]=%lf, z[i]=%lf\n", i, y[i], z[i]);
                }
            }
        }

        /************** || block column decomposition (collective only) || *******************/
        int n_col = n_wier; // each process processes ncol columns

        // z is initialized for all ranks
        for (i = 0; i < WYMIAR; i++)
            z[i] = 0.0;

        // local a is initialized to zero - now local a stores several columns (not rows as before)
        for (i = 0; i < WYMIAR * n_col; i++)
            a_local[i] = 0.0;

        // Create a derived datatype for a block of n_col columns
        double *col_data;
        int total_cols = size * n_col; // liczba kolumn do rozdzielenia

        if (rank == root)
        {
            col_data = (double *)malloc(WYMIAR * n_col * size * sizeof(double));

            for (j = 0; j < total_cols; j++)
            {
                // Dla każdej kolumny j kopiujemy wszystkie wiersze
                for (i = 0; i < WYMIAR; i++)
                {
                    // macierz a (wierszowa) -> bufor col_data (dla zachowania ciągłości - c * WYMIAR + i)
                    col_data[j * WYMIAR + i] = a[i * WYMIAR + j];
                }
            }
        }

        // if (rank == root)
        // {
        //   printf("Kolumnowa: WYMIAR=%d, size=%d, n_col=%d, total_cols=%d\n", WYMIAR, size, n_col, total_cols);
        // }

        // Rozpraszamy bloki kolumn – każdy proces dostaje WYMIAR * n_col elementów
        MPI_Scatter(col_data, WYMIAR * n_col, MPI_DOUBLE, a_local, WYMIAR * n_col, MPI_DOUBLE, root, MPI_COMM_WORLD);

        if (rank == root)
        {
            free(col_data);
            printf("Starting MPI matrix-vector product with block column decomposition!\n");
            t1 = MPI_Wtime();
        }

        // loop over rows (optimal since matrix a stored row major)
        for (i = 0; i < n; i++)
        {
            double t = 0.0;
            int jj = rank * n_col; // jj – początek fragmentu wektora x odpowiadający danym tego procesu

            for (j = 0; j < n_col; j++)
            {
                t += a_local[j * WYMIAR + i] * x[jj + j];
            }
            // printf("rank %d: row %d, final y %lf\n", rank, i, t);
            z[i] = t;
        }

        // I. Reduce - for each element of z reduction is necessary to get the final result
        // Ia. version with loop over ranks and reductions for n_col long chunks

        // Ib. simple version with reductions for individual elements of z (loop over rows)
        // end I. Reduce

        if (rank == root)
        {
            MPI_Reduce(
                MPI_IN_PLACE, z, WYMIAR, MPI_DOUBLE, MPI_SUM, root,
                MPI_COMM_WORLD); // MPI_IN_PLACE - dane nad którymi operujemy, są już umieszczone w docelowym buforze
        }
        else
        {
            MPI_Reduce(z, NULL, WYMIAR, MPI_DOUBLE, MPI_SUM, root,
                       MPI_COMM_WORLD); // NULL, bo wynik zostanie przesłany do procesu root
        }

        // MPI_Allreduce(MPI_IN_PLACE, z, WYMIAR, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // II. All-to-all
        // WARNING: All-to-all requires large MPI buffers (check matrix size WYMIAR in case of errors)

        // All-to-all requires also synchronisation
        /* MPI_Barrier(MPI_COMM_WORLD); */

        // Alltoall
        // ...

        // All-to-all requires synchronisation also at this point
        /* MPI_Barrier(MPI_COMM_WORLD); */

        // Alltoall exchanges chunks of vector z - each process stores nproc chunks
        // with size n_col obtained from other processes, to get the final result
        // at the proper positions from z(rank*n_col) to z(rank*n_col+n_col-1)
        // each process has to sum all nproc chunks (it is assumed that the owned chunk
        // retains the calculated value as the initial value for summation
        // (remark: nproc is equal to size in MPI nomenclature, not to be mixed up
        // with the size of parts of z equal to n_col)

        /* for(i=0; i<size; i++){ */
        /*   if(i!=rank){ */
        /* 	for(j=0;j<n_col;j++){ */
        /* 	  z[rank*n_col+j] += z[i*n_col+j]; */
        /* 	} */
        /*   } */
        /* } */

        // end II. All-to-all

        // just to measure time
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == root)
        {
            t1 = MPI_Wtime() - t1;
            printf("Werja rownolegla MPI z dekompozycją kolumnową blokową\n");
            printf("\tczas wykonania: %lf, Gflop/s: %lf, GB/s> %lf\n", t1, 2.0e-9 * ROZMIAR / t1,
                   (1.0 + 1.0 / n) * 8.0e-9 * ROZMIAR / t1);
        }

        // testing - switch on after completing calculations and communcation
        if (rank == root)
        {

            for (i = 0; i < WYMIAR; i++)
            {
                if (fabs(y[i] - z[i]) > 1.e-9 * z[i])
                {
                    printf("Blad! i=%d, y[i]=%lf, z[i]=%lf - complete the code for column decomposition\n", i, y[i],
                           z[i]);
                    break;
                    // printf("Blad! i=%d, y[i]=%lf, z[i]=%lf\n", i, y[i], z[i]);
                }
            }
        }
    }

    MPI_Finalize();

    return (0);
}
