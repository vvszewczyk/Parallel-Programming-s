#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

// Wierszowa
void decomposition1(double sum, double a[WYMIAR][WYMIAR])
{
	#pragma omp parallel for default(none) reduction(+:sum) shared(a) schedule(static, 2) ordered
	for (int i = 0; i < WYMIAR; i++)
	{
		#pragma omp ordered
		for (int j = 0; j < WYMIAR; j++)
		{
			sum += a[i][j];
			printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
		}
		printf("\n");
	}
	printf("Suma wyrazów tablicy równolegle: %lf\n", sum);
}

// Kolumnowa 1
void decomposition2(double sum, double a[WYMIAR][WYMIAR])
{
	for (int i = 0; i < WYMIAR; i++)
	{
		#pragma omp parallel for default(none) reduction(+:sum) shared(a, i) schedule(dynamic) ordered
		for (int j = 0; j < WYMIAR; j++)
		{
			#pragma omp ordered
			{
				sum += a[i][j];
				printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
			}
		}
		printf("\n");
	}
	printf("Suma wyrazów tablicy równolegle: %lf\n", sum);
}


// Kolumnowa 2
void decomposition3(double sum, double a[WYMIAR][WYMIAR])
{
	#pragma omp parallel default(none) shared(a, sum)
	{
		double localSum = 0.0;
		#pragma omp for schedule(static) ordered
		for (int j = 0; j < WYMIAR; j++)
		{
			#pragma omp ordered
			for (int i = 0; i < WYMIAR; i++)
			{
				localSum += a[i][j];
				printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
			}
			printf("\n");
		}
		#pragma omp critical(sum) // atomic
		sum += localSum;
	}
	printf("Suma wyrazów tablicy równolegle: %lf\n", sum);
}

// 2D
void decomposition4(double sum, double a[WYMIAR][WYMIAR])
{
	omp_set_num_threads(2); // wiersze

	#pragma omp parallel for default(none) reduction(+:sum) shared(a) schedule(static, 3) ordered
	for (int i = 0; i < WYMIAR; i++)
	{
		int id_w = omp_get_thread_num(); // Wątek zewnętrzny
		omp_set_num_threads(2); // kolumny

		#pragma omp parallel for default(none) reduction(+:sum) shared(a, i, id_w) schedule(static, 3) ordered
		for (int j = 0; j < WYMIAR; j++)
		{
			#pragma omp ordered
			{
				sum += a[i][j];
				printf("(%1d,%1d)-W_%1d,%1d ", i, j, id_w, omp_get_thread_num());
			}
		}
		#pragma omp ordered
		printf("\n");
	}
	printf("Suma wyrazów tablicy równolegle: %lf\n", sum);
}

// Wierszowy
void decomposition5(double sum, double a[WYMIAR][WYMIAR])
{
	double sharedSum[WYMIAR] = { 0.0 };
	omp_set_num_threads(4);

	for (int j = 0; j < WYMIAR; j++)
	{
		printf("Kolumna %d\n", j);

		#pragma omp parallel for default(none) reduction(+:sum) shared(a, j, sharedSum) schedule(static, 1) ordered
		for (int i = 0; i < WYMIAR; i++)
		{
			int id_w = omp_get_thread_num();
			#pragma omp ordered
			{
				sharedSum[i] += a[i][j];
				printf("(%1d,%1d)-W_%1d\n", i, j, id_w);
			}
		}
		printf("\n");
	}

	for (int i = 0; i < WYMIAR; i++)
	{
		sum += sharedSum[i];
	}

	printf("Suma wyrazów tablicy równolegle: %lf\n", sum);
}

int main()
{
	omp_set_num_threads(3);
	double a[WYMIAR][WYMIAR];

	for (int i = 0; i < WYMIAR; i++) 
	{
		for (int j = 0; j < WYMIAR; j++) 
		{
			a[i][j] = 1.02 * i + 1.01 * j;
		}
	}

	// podwójna pętla - sekwencyjnie
	double suma = 0.0;
	for (int i = 0; i < WYMIAR; i++)
	{
		for (int j = 0; j < WYMIAR; j++)
		{
			suma += a[i][j];
		}
	}

	printf("Suma wyrazów tablicy: %lf\n", suma);

	omp_set_nested(1); // Możliwość tworzenia zagnieżdżonych obszarów równoległych

	// podwójna pętla - docelowo równolegle
	double suma_parallel = 0.0;

	decomposition1(suma_parallel, a);
	printf("\n\n");
	decomposition2(suma_parallel, a);
	printf("\n\n");
	decomposition3(suma_parallel, a);
	printf("\n\n");
	decomposition4(suma_parallel, a);
	printf("\n\n");
	decomposition5(suma_parallel, a);
}
