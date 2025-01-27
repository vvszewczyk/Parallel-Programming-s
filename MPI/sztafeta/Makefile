# kompilator c
CCOMP = /usr/bin/mpicc

# konsolidator
LOADER = /usr/bin/mpicc

# program uruchomieniowy MPI
MPI_run = /usr/bin/mpiexec -oversubscribe

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lm
LIB = -lm

all: ring run 

run: ring
	$(MPI_run) -np 4 ./ring

# zaleznosci i komendy
ring : ring.o
	$(LOADER) $(OPT) ring.o -o ring $(LIB)

ring.o: ring.c 
	$(CCOMP) -I/usr/include/openmpi -c $(OPT) ring.c
clean:
	rm -f *.o ring
