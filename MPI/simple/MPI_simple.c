#include "mpi.h"
#include <bits/local_lim.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // rank - id procesu
    // size - rozmiar komunikatora (liczba procesów )
    // MPI_COMM_WORLD - komunikator, służy do ujednoznacznienia (jeden proces może mieć różne id w ramach różnych
    // komunikatorów)
    int rank, ranksent, size, source, dest, tag_rank, tag_host;
    MPI_Status status;

    char host_address[HOST_NAME_MAX + 1];
    char recieved_address[HOST_NAME_MAX + 1];

    MPI_Init(&argc, &argv);               // Przesłanie argumentów do środowiska MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Uzyskanie rangi
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Uzyskanie liczby procesów

    gethostname(host_address, sizeof(host_address)); // Pobiera adres hosta

    if (size > 1)
    {
        if (rank != 0)
        {
            // --- Proces wysyłający ---
            dest = 0;
            tag_rank = 0;
            tag_host = 1;

            // Wysyłanie rangi
            MPI_Send(&rank, 1, MPI_INT, dest, tag_rank,
                     MPI_COMM_WORLD); // MPI_INT - obiekt który zawiera definicję liczby całkowitej na potrzeby MPI (np.
                                      // info o systemie 32/64 bitowym)

            // Wysyłanie nazwy nadawcy
            MPI_Send(host_address, strlen(host_address) + 1, MPI_CHAR, dest, tag_host, MPI_COMM_WORLD);
        }
        else
        {
            // --- Proces odbierający 0 ---
            for (int i = 1; i < size; i++)
            {
                // Odbieranie rangi
                MPI_Recv(&ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

                // Odbieranie nazwy
                MPI_Recv(recieved_address, sizeof(recieved_address), MPI_CHAR, status.MPI_SOURCE, 1, MPI_COMM_WORLD,
                         &status);
                printf("Dane od procesu o randze (status.MPI_SOURCE ->) %d: %d (i=%d)\n", status.MPI_SOURCE, ranksent,
                       i);
                printf("Nazwa hosta procesu o randze %d: %s\n", ranksent, recieved_address);
            }
        }
    }
    else
    {
        printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
    }

    MPI_Finalize(); // Pozwala wyczyścić strukturę danych MPI po wcześniejszych uruchomieniach programu żeby nie było
                    // błędów

    return 0;
}
