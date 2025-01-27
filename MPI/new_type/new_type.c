#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 4

typedef struct
{
    double g;
    char character;
    float value[3];
} SE;

int main(int argc, char **argv)
{
    int id, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    SE records[ARRAY_SIZE];
    if (id == 0)
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            records[i].g = i * i;
            records[i].character = 'A' + i;
            for (int j = 0; j < 3; j++)
            {
                records[i].value[j] = i + j * 0.5;
            }
        }
    }

    MPI_Datatype record_type, resized_record_type;
    int block_lengths[3] = {1, 1, 3};                          // Liczba elementów w każdym polu
    MPI_Datatype types[3] = {MPI_DOUBLE, MPI_CHAR, MPI_FLOAT}; // Typy dla każdego pola
    MPI_Aint offsets[3], base_address, lb, extent;

    // Pobieranie adresów pól pierwszej struktury (zwraca MPI_Aint)
    // int MPI_Get_address(const void *location, MPI_Aint *address)
    MPI_Get_address(&records[0].g, &offsets[0]);
    MPI_Get_address(&records[0].character, &offsets[1]);
    MPI_Get_address(&records[0].value[0], &offsets[2]);

    // Obliczanie przesunięć każdego pola struktury względem pierwszego pola (base_address)
    base_address = offsets[0];
    for (int i = 0; i < 3; i++)
    {
        offsets[i] -= base_address;
    }

    // Tworzenie - Zamiast przesyłać każde pole struktury osobno, tworzymy jeden typ MPI, który reprezentuje całą
    // strukturę.
    /*
    int MPI_Type_create_struct(
    int count,                // Liczba elementów w strukturze
    const int *block_lengths, // Tablica długości bloków
    const MPI_Aint *offsets,  // Tablica przesunięć
    const MPI_Datatype *types,// Tablica typów MPI dla każdego bloku
    MPI_Datatype *newtype     // Wskaźnik na nowy typ MPI
    )
    */
    MPI_Type_create_struct(3, block_lengths, offsets, types, &record_type);
    MPI_Type_commit(&record_type);

    // Rozszerzanie typu o wyrównanie
    /*
    int MPI_Type_get_extent(
    MPI_Datatype datatype,    // Typ MPI
    MPI_Aint *lb,             // Dolna granica
    MPI_Aint *extent          // Zakres
    )
    */
    MPI_Type_get_extent(record_type, &lb,
                        &extent); // Oblicza zakres (extent) i dolną granicę (lower bound, lb) dla nowego typu MPI

    /*
    int MPI_Type_create_resized(
    MPI_Datatype oldtype,     // Stary typ MPI
    MPI_Aint lb,              // Dolna granica (zwykle 0)
    MPI_Aint extent,          // Zakres nowego typu
    MPI_Datatype *newtype     // Nowy typ MPI
    )
    */
    MPI_Type_create_resized(
        record_type, lb, extent,
        &resized_record_type); // Tworzy nowy typ MPI na bazie istniejącego, ale zmienia jego rozmiar (extent).
    MPI_Type_commit(&resized_record_type);

    if (id == 0)
    {
        // Wysyłanie danych do procesu 1
        MPI_Send(records, ARRAY_SIZE, resized_record_type, 1, 0, MPI_COMM_WORLD);
        printf("Proces 0: Wysłano %d rekordów\n", ARRAY_SIZE);
    }
    else if (id == 1)
    {
        // Odbieranie danych w procesie 1
        SE received_records[ARRAY_SIZE];
        MPI_Recv(received_records, ARRAY_SIZE, resized_record_type, 0, 0, MPI_COMM_WORLD, &status);

        printf("Proces 1: Odebrano %d rekordów:\n", ARRAY_SIZE);
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("Rekord %d: g=%.2f, character=%c, value=[%.2f, %.2f, %.2f]\n", i, received_records[i].g,
                   received_records[i].character, received_records[i].value[0], received_records[i].value[1],
                   received_records[i].value[2]);
        }
    }

    MPI_Type_free(&record_type);
    MPI_Type_free(&resized_record_type);

    MPI_Finalize();
    return 0;
}
