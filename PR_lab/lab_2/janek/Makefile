# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
#OPT = -g -DDEBUG -O0
# wersja zoptymalizowana do mierzenia czasu
OPT = -g

# pliki naglowkowe
INC = -I../../pomiar_czasu

# biblioteki
LIB = -L../../pomiar_czasu -lpomiar_czasu -lm

all: janek


janek: janek.o 
	$(LOADER) $(OPT) janek.o -o result $(LIB)

janek.o: janek.c ../../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) janek.c $(INC) 

clean:
	rm -f *.o fork clone