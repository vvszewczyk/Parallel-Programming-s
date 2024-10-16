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
#define ILOSC_ITERACJI 100000

int funkcja_watku(void* argument) 
{
    int zmienna_lokalna = *((int*)argument);
    for (int i = 0; i < ILOSC_ITERACJI; i++) 
    {
        zmienna_globalna++;
        zmienna_lokalna++;
    }
    printf("Zmienna globalna: %d\n Zmienna lokalna: %d\n\n", zmienna_globalna, zmienna_lokalna);
    return 0;
}

int main() 
{
    void *stos1, *stos2;
    pid_t pid1, pid2;
    int zmienna_lokalna1 = 0;
  
    stos1 = malloc(ROZMIAR_STOSU);
    stos2 = malloc(ROZMIAR_STOSU);

    if (stos1 == 0 || stos2 == 0) 
    {
        printf("Proces nadrzędny - błąd alokacji stosu\n");
        exit(1);
    }

    inicjuj_czas();  
    pid1 = clone(funkcja_watku, (void *)stos1 + ROZMIAR_STOSU, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &zmienna_lokalna1);
    pid2 = clone(funkcja_watku, (void *)stos2 + ROZMIAR_STOSU, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &zmienna_lokalna1);
    drukuj_czas(); 

    waitpid(pid1, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);

    printf("Ostateczna zmienna globalna: %d\n", zmienna_globalna);

    free(stos1);
    free(stos2);
}
