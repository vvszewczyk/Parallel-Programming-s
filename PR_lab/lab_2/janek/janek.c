#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE 1024*64
#define NUM_ITERATIONS 100000

// Zmienna globalna
int global_var = 0;

// Funkcja wykonywana przez wątki
int thread_function(void *arg) {
    int local_var = *((int *)arg);  // Przekazanie wskaźnika na lokalną zmienną

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        global_var++;  // Zwiększanie zmiennej globalnej
        local_var++;   // Zwiększanie zmiennej lokalnej
    }

    // Wypisywanie wartości zmiennych bezpośrednio w wątku po zakończeniu pętli
    printf("WĄTEK ZAKOŃCZONY: Zmienna lokalna = %d, Zmienna globalna = %d\n", local_var, global_var);

    return 0;
}

int main() {
    void *stack1, *stack2;
    int local_var1 = 0;
    pid_t pid1, pid2;

    // Alokacja stosów dla wątków
    stack1 = malloc(STACK_SIZE);
    stack2 = malloc(STACK_SIZE);
    if (stack1 == 0|| stack2 == 0) {
        exit(1);
    }


    // Tworzenie dwóch wątków równolegle za pomocą clone()
    pid1 = clone(thread_function, (void *)stack1 + STACK_SIZE, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &local_var1);
    pid2 = clone(thread_function, (void *)stack2 + STACK_SIZE, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &local_var1);

    // Oczekiwanie na zakończenie wątków za pomocą waitpid
    waitpid(pid1, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);
    

    // Zwolnienie pamięci
    free(stack1);
    free(stack2);

    return 0;
}