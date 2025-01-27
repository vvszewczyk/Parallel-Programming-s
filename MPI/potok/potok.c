#include "mpi.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEXT 100
#define NUM_MESSAGES 4

typedef struct
{
    char text[MAX_TEXT];
    int length;
} SE;

void reverse_text(SE *data)
{
    int n = data->length;
    for (int i = 0; i < n / 2; i++)
    {
        char temp = data->text[i];
        data->text[i] = data->text[n - i - 1];
        data->text[n - i - 1] = temp;
    }
}

void replace_spaces(SE *data)
{
    int n = data->length;
    for (int i = 0; i < n; i++)
    {
        if (data->text[i] == ' ')
        {
            data->text[i] = '_';
        }
        else if (data->text[i] == '_')
        {
            data->text[i] = ' ';
        }
    }
}

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

    bufor = (void *)malloc(bufor_size);

    SE data;

    if (id == 0)
    {
        char *localities[NUM_MESSAGES] = {"Potok Gorny", "Laczki Brzeskie", "Bielsko Biala", "Kedzierzyn Kozle"};

        for (int i = 0; i < NUM_MESSAGES; i++)
        {
            strcpy(data.text, localities[i]);
            data.length = strlen(data.text);

            position = 0;
            MPI_Pack(data.text, MAX_TEXT, MPI_CHAR, bufor, bufor_size, &position, MPI_COMM_WORLD);
            MPI_Pack(&data.length, 1, MPI_INT, bufor, bufor_size, &position, MPI_COMM_WORLD);

            MPI_Send(bufor, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
            usleep(10000);
            printf("Proces = %d: Wysłano -> string = %s, dlugosc = %d\n", id, data.text, data.length);
        }
    }
    else
    {
        for (int i = 0; i < NUM_MESSAGES; i++)
        {
            MPI_Recv(bufor, bufor_size, MPI_PACKED, id - 1, 0, MPI_COMM_WORLD, &status);

            position = 0;
            MPI_Unpack(bufor, bufor_size, &position, data.text, MAX_TEXT, MPI_CHAR, MPI_COMM_WORLD);
            MPI_Unpack(bufor, bufor_size, &position, &data.length, 1, MPI_INT, MPI_COMM_WORLD);

            if (id % 2 == 0)
            {
                reverse_text(&data);
            }
            else
            {
                replace_spaces(&data);
            }

            usleep(10000);
            printf("Proces = %d: Przetworzono -> string = %s, dlugosc = %d\n", id, data.text, data.length);

            if (id < size - 1)
            {
                position = 0;
                MPI_Pack(data.text, MAX_TEXT, MPI_CHAR, bufor, bufor_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&data.length, 1, MPI_INT, bufor, bufor_size, &position, MPI_COMM_WORLD);
                MPI_Send(bufor, position, MPI_PACKED, id + 1, 0, MPI_COMM_WORLD);
            }
        }
    }

    free(bufor);
    MPI_Finalize();
    return 0;
}
