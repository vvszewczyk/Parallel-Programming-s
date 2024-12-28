# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

#OPT = -g -DDEBUG

OPT = -fopenmp

all: openmp_petle_simple openmp_petle

openmp_petle_simple: openmp_petle_simple.c
	$(CCOMP) $(OPT) openmp_petle_simple.c -o openmp_petle_simple

openmp_petle: openmp_petle.c
	$(CCOMP) $(OPT) openmp_petle.c -o openmp_petle

clean:
	rm -f openmp_petle_simple openmp_petle
