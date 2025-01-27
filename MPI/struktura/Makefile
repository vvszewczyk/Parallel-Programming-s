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

all: struct run 

run: struct
	$(MPI_run) -np 4 ./struct

# zaleznosci i komendy
struct : struct.o
	$(LOADER) $(OPT) struct.o -o struct $(LIB)

struct.o: struct.c 
	$(CCOMP) -I/usr/include/openmpi -c $(OPT) struct.c
clean:
	rm -f *.o struct
