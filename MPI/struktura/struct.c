#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"

#define MAX_TEXT 20

typedef struct
{
    char name[MAX_TEXT];
    int number;
    double g;
} SE;

int main(int argc, char **argv)
{
    int id, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int bufor_size, pack_size, position;
    void *bufor;

    MPI_Pack_size(MAX_TEXT, MPI_CHAR, MPI_COMM_WORLD, &pack_size);
    bufor_size = pack_size;
    MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &pack_size);
    bufor_size += pack_size;
    MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &pack_size);
    bufor_size += pack_size;

    bufor = (void *)malloc(bufor_size);
    position = 0; // podczas przekazywania bufor nam się nie będzie zmieniał tylko position

    SE data;

    if (id == 0)
    {
        strcpy(data.name, "Wojtek");
        data.number = 10;
        data.g = 2.0;

        printf("Proces = %d: Pakowanie struktury do bufora i wysyłanie\n", id);

        // Pakowanie do bufora
        MPI_Pack(data.name, MAX_TEXT, MPI_CHAR, bufor, bufor_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.number, 1, MPI_INT, bufor, bufor_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.g, 1, MPI_DOUBLE, bufor, bufor_size, &position, MPI_COMM_WORLD);

        MPI_Send(bufor, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
        printf("Proces = %d: Wysłano strukturę -> Name: %s, Number: %d, g: %.2f\n", id, data.name, data.number, data.g);
    }
    else
    {
        printf("Proces = %d: Otrzymywanie i rozpakowanie struktury z bufora\n", id);
        MPI_Recv(bufor, bufor_size, MPI_PACKED, id - 1, 0, MPI_COMM_WORLD, &status);

        position = 0;

        MPI_Unpack(bufor, bufor_size, &position, data.name, MAX_TEXT, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(bufor, bufor_size, &position, &data.number, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(bufor, bufor_size, &position, &data.g, 1, MPI_DOUBLE, MPI_COMM_WORLD);

        printf("Proces = %d: Odebrano strukturę -> Name: %s, Number: %d, g: %.2f\n", id, data.name, data.number,
               data.g);

        strcat(data.name, "*");
        data.number += 2;
        data.g += 10.0;

        if (id < size - 1)
        {
            printf("Proces = %d: Pakowanie zmodyfikowanej struktury do bufora i wysyłanie\n", id);
            position = 0;
            MPI_Pack(data.name, MAX_TEXT, MPI_CHAR, bufor, bufor_size, &position, MPI_COMM_WORLD);
            MPI_Pack(&data.number, 1, MPI_INT, bufor, bufor_size, &position, MPI_COMM_WORLD);
            MPI_Pack(&data.g, 1, MPI_DOUBLE, bufor, bufor_size, &position, MPI_COMM_WORLD);

            MPI_Send(bufor, position, MPI_PACKED, id + 1, 0, MPI_COMM_WORLD);
            printf("Proces %d: Wysłano strukturę -> Name: %s, Number: %d, g: %.2f\n", id, data.name, data.number,
                   data.g);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    free(bufor);
    return 0;
}
