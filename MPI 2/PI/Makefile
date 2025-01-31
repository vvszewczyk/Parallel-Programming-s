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
OUT = pi
PROC = 6

all: $(OUT) run 

run: $(OUT)
	$(MPI_run) -np $(PROC) ./$(OUT)

# zaleznosci i komendy
$(OUT): oblicz_PI.c
	$(LOADER) $(OPT) oblicz_PI.c -o $(OUT) $(LIB)

clean:
	rm -f *.o pi
