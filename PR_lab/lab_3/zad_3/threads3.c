#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define p 2

struct joe
{
    int id;
    double a, b, result;
};

void * threadFunction(void * arg)
{
    struct joe *kamala = ((struct joe * )arg);
    struct joe kamala2 = *((struct joe * )arg);
    kamala->result = pow(kamala->b, kamala->a);
    printf("Srtuct id: %d, pow(b, a): %lf\n", kamala->id, kamala->result);

    pthread_exit(NULL);
}


int main()
{
    struct joe biden = {1, 3.14, 2.7, 0.0};
    struct joe mama = {2, 1.1, 9.9, 0.0};

    struct joe joes[p];
    joes[0] = biden;
    joes[1] = mama;

    pthread_t threads[p];

    for(int i = 0; i < p; i++)
    {
        pthread_create(&threads[i], NULL, threadFunction, &joes[i]);
    }

    double total = 0.0;

    for(int i = 0; i < p; i++)
    {
        pthread_join(threads[i], NULL);
        total += joes[i].result;
    }
    

    printf("\nResult: %lf\n", total);

    pthread_exit(NULL);
}