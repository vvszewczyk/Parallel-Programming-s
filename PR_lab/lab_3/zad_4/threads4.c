#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define p 3

void * threadFunction(void * arg)
{
    int localID = *((int*)arg);
    pthread_attr_t localAttr;
    size_t localStack;
    int localState;

    int tab[10000000] = {0}; // około 41 MB

    pthread_getattr_np(pthread_self(), &localAttr);
    pthread_attr_getstacksize(&localAttr, &localStack);
    pthread_attr_getdetachstate(&localAttr, &localState);

    struct sched_param localParam;
    int policy;
    pthread_getschedparam(pthread_self(), &policy, &localParam);

    int cpu = sched_getcpu();

    printf("\nThread number: %d, stack size: %zu, state: %s, priority: %d, CPU number: %d\n", localID, localStack, localState == PTHREAD_CREATE_DETACHED ? "detached" : "attached", localParam.sched_priority, cpu);

    pthread_attr_destroy(&localAttr);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[p];
    pthread_attr_t attrs[p];
    int pids[p];

    struct sched_param param;

    size_t stackSize = 50 * 1024 * 1024; // 50 MB

    for(int i = 0; i < p; i++)
    {
        pids[i] = i;
        pthread_attr_init(&attrs[i]);
        pthread_attr_setstacksize(&attrs[i], stackSize);

        // Polityka Round-Robin
        pthread_attr_setschedpolicy(&attrs[i], SCHED_RR);

        param.sched_priority = 20 + i;
        pthread_attr_setschedparam(&attrs[i], &param);

        // Ustawienie aby atrybuty miały priorytet wątków !!!wymagane do działania z priorytetami!!!
        pthread_attr_setinheritsched(&attrs[i], PTHREAD_EXPLICIT_SCHED);

        if (i == 0)
        {
            pthread_attr_setdetachstate(&attrs[i], PTHREAD_CREATE_DETACHED);
        }
    }

    for(int i = 0; i < p; i++)
    {
        pthread_create(&threads[i], &attrs[i], threadFunction, &pids[i]);

        // "afiaiczność" - przydzielenie wątkowi konkretnego procesora 
        cpu_set_t CPU;
        CPU_ZERO(&CPU);
        CPU_SET(i + 11, &CPU);

        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &CPU);

    }

    for(int i = 1; i < p; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < p; i++)
    {
        pthread_attr_destroy(&attrs[i]);
    }

    pthread_exit(NULL);
}