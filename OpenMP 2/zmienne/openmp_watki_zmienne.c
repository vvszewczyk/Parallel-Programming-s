#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
#ifdef _OPENMP
    printf("\nKompilator rozpoznaje dyrektywy OpenMP\n");
#endif

    int a_shared = 1;
    int b_private = 2;
    int c_firstprivate = 3;
    int e_atomic = 5;
    static int f_threadprivate = -1;       // kazdy watek ma swoja kopie tej zmiennej
#pragma omp threadprivate(f_threadprivate) // nie trzeba jej przesylac

    printf("przed wejsciem do obszaru rownoleglego -  nr_threads %d, thread ID %d\n", omp_get_num_threads(),
           omp_get_thread_num());
    printf("\ta_shared \t= %d\n", a_shared);
    printf("\tb_private \t= %d\n", b_private);
    printf("\tc_firstprivate \t= %d\n", c_firstprivate);
    printf("\te_atomic \t= %d\n", e_atomic);

#pragma omp parallel default(none) shared(a_shared, e_atomic) private(b_private) firstprivate(c_firstprivate)
    {
        b_private = 2;
        int d_local_private;
        d_local_private = a_shared + c_firstprivate;

        int a_shared_local = 0;
        int e_atomic_local = 0;

        // WAW, operacja jest wykonywana lokalnie
        for (int i = 0; i < 10; i++)
        {
            a_shared_local++;
        }

// WAR dla zmiennej a_shared, co może prowadzić do różnych wartości zmiennej d_local_private w różnych uruchomieniach
// programu i różnych wątkach
//(założeniem jest, że wartość ta powinna być taka sama w każdym wątku i przy każdym uruchomieniu programu)
#pragma omp barrier

// WAW, zapobiega wyścigowi
#pragma omp critical
        a_shared += a_shared_local;

        for (int i = 0; i < 10; i++)
        {
            c_firstprivate += omp_get_thread_num();
        }

        // WAW, operacja jest wykonywana lokalnie
        for (int i = 0; i < 10; i++)
        {
            e_atomic_local += omp_get_thread_num();
        }

// WAW, zapobiega wyścigowi
#pragma omp atomic
        e_atomic += e_atomic_local;

#pragma omp barrier

// Sekcja krytyczna zapobiega przeplataniu się wyników
#pragma omp critical
        {
            printf("\nw obszarze równoległym: aktualna liczba watkow %d, moj ID %d\n", omp_get_num_threads(),
                   omp_get_thread_num());
            printf("\ta_shared \t= %d\n", a_shared);
            printf("\tb_private \t= %d\n", b_private);
            printf("\tc_firstprivate \t= %d\n", c_firstprivate);
            printf("\td_local_private = %d\n", d_local_private);
            printf("\te_atomic \t= %d\n", e_atomic);
        }

        f_threadprivate = omp_get_thread_num();
    }

#pragma omp parallel default(none)
    {
#pragma omp critical
        {
            printf("\nDrugi obszar równoległy: aktualna liczba watkow %d, moj ID %d\n", omp_get_num_threads(),
                   omp_get_thread_num());
            printf("\tf_threadprivate \t= %d\n", f_threadprivate);
        }
    }

    printf("\npo zakonczeniu obszaru rownoleglego:\n");
    printf("\ta_shared \t= %d\n", a_shared);
    printf("\tb_private \t= %d\n", b_private);
    printf("\tc_firstprivate \t= %d\n", c_firstprivate);
    printf("\te_atomic \t= %d\n", e_atomic);
}
