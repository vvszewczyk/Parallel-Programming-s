CCOMP = gcc
OPT = -g -O0

all: program call


program: pub_sym_1.o
	$(CCOMP) $(OPT) pub_sym_1.o -o result 

program.o: pub_sym_1.c
	$(CCOMP) -c pub_sym_1.c 

call:
	./result

clean:
	rm -rf *.o result