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

all: new_type run 

run: new_type
	$(MPI_run) -np 4 ./new_type

# zaleznosci i komendy
new_type : new_type.o
	$(LOADER) $(OPT) new_type.o -o new_type $(LIB)

new_type.o: new_type.c 
	$(CCOMP) -I/usr/include/openmpi -c $(OPT) new_type.c
clean:
	rm -f *.o new_type
