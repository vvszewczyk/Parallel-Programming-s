#Aby uruchomic kilka programow na raz mozna zdefiniowac glowna regule tj.
# all: program1 program2
# gdzie programy sa zdefiniowane mniej wiecej tj. output ponizej
# Druga opcja jest uruchamianie obu programow z poziomu konsoli
BUFOR=gcc
OPT1=-c

output: main.o hello.o
	$(BUFOR) main.o hello.o -o output

main.o: main.c
	$(BUFOR) $(OPT1) main.c

hello.o: hello.c hello.h
	$(BUFOR) $(OPT1) hello.c

clean:
	rm -rf *.o
