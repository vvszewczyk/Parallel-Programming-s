#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

int zmienna_globalna = 0;

#define ROZMIAR_STOSU 1024*64


int funkcja_watku(void* argument) 
{
    int thread_id = *(int*)argument;

    zmienna_globalna++;

    char thread_id_str[10];
    snprintf(thread_id_str, sizeof(thread_id_str), "%d", thread_id); //konwersja id na string: bufor, ilosc znakow, typ, numer

    char* args[] = {"./program", thread_id_str, NULL};

    int wynik;
    double start_time = czas_zegara();
    //wynik = execv("./program", NULL);
    wynik = execv(args[0], args);
    double end_time = czas_zegara();

    if (wynik == -1) 
    { 
      printf("Proces potomny nie wykonał programu\n");
    }


    return 0;
}


int main() 
{
    void* stos;
    pid_t pid;
    int i; 

    stos = malloc(ROZMIAR_STOSU);
    if (stos == 0) {
        printf("Proces nadrzędny - błąd alokacji stosu\n");
        exit(1);
    }

    inicjuj_czas();

    for (i = 0; i < 10; i++) 
    {
        int* thread_id_ptr = malloc(sizeof(int));
        *thread_id_ptr = i;

        pid = clone(&funkcja_watku, (void *)stos + ROZMIAR_STOSU, CLONE_FS | CLONE_SIGHAND | CLONE_VM, thread_id_ptr);
        waitpid(pid, NULL, __WCLONE);
        free(thread_id_ptr);
    }

    drukuj_czas();
    printf("%d\n", zmienna_globalna);

    free(stos);
    return 0;
}
