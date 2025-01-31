#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SCALAR double

#ifndef M_PI // standardy C99 i C11 nie wymagaja definiowania stalej M_PI
#define M_PI (3.14159265358979323846)
#endif

// program obliczania przyblizenia PI za pomoca wzoru Leibniza
// PI = 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + itd.)
int main(int argc, char **argv)
{
    int max_number = 0;
    int root = 0;
    int start, end, stride;
    int id, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (id == root)
    {
        printf("Podaj maksymalna liczbe wyrazow do obliczenia przyblizenia PI\n");
        scanf("%d", &max_number);
    }

    MPI_Bcast(&max_number, 1, MPI_INT, root, MPI_COMM_WORLD);

    int el_on_p = (int)ceil((double)max_number / size);
    start = id * el_on_p;
    end = el_on_p * (id + 1);
    stride = 1;
    if (end > max_number)
    {
        end = max_number;
    }

    // wzor: PI/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 itd. itp.
    SCALAR sum_plus = 0.0;
    SCALAR sum_minus = 0.0;
    SCALAR local_sum = 0.0;

    printf("Proces ID = %d, start = %d, end = %d, stride = %d\n", id, start, end, stride);

    for (int i = start; i < end; i += stride)
    {
        int j = 1 + 4 * i;
        sum_plus += 1.0 / j;
        sum_minus += 1.0 / (j + 2.0);
        // printf("PI obliczone: %20.15lf, aktualna poprawka: %20.15lf\n",
        //   	 4*(suma_plus-suma_minus), 4.0/j-4.0/(j+2.0));
    }
    local_sum = 4 * (sum_plus - sum_minus);

    SCALAR global_sum = 0.0;
    // MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if (id == root)
    {
        //  t = MPI_Wtime() - t;
        printf("PI obliczone: \t\t\t%20.15lf\n", global_sum);
        printf("PI z biblioteki matematycznej: \t%20.15lf\n", M_PI);
    }

    MPI_Finalize();
    //  printf("Czas obliczeĹ: %lf\n", t);
}
