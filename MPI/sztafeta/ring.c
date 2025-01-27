#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char **argv)
{
    int id, size, prev, next, data;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    prev = id - 1; // prev_map[id];
    next = id + 1; // next_map[id];

    if (size < 3)
    {
        if (id == 0)
        {
            printf("Program wymaga co najmniej 3 procesów.\n");
        }
        MPI_Finalize();
        return 0;
    }

    data = 0;

    if (id == 0) // Proces początkowy
    {
        printf("Proces = %d: data = %d\n", id, data);

        MPI_Send(&data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Proces = %d: przesłano data = %d do procesu = %d\n", id, data, next);
    }
    else if (id == size - 1) // Proces końcowy
    {
        MPI_Recv(&data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        printf("Proces = %d: odebrano data = %d od procesu = %d\n", id, data, prev);

        data += 100;
        printf("Proces = %d: modyfikacja data = %d\n", id, data);

        printf("Proces = %d: KONIEC data = %d\n", id, data);
    }
    else // Proces środkowy
    {
        printf("Proces = %d: data = %d\n", id, data);

        MPI_Recv(&data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        printf("Proces = %d: odebrano data = %d od procesu %d\n", id, data, prev);

        data += 10;
        printf("Proces = %d: modyfikacja data = %d\n", id, data);

        MPI_Send(&data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Proces = %d: przesłano data = %d do procesu = %d\n", id, data, next);
    }

    MPI_Finalize();

    return 0;
}
