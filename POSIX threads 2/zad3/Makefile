CCOMP = gcc

all: program call


program: pub_sym_1.o
	$(CCOMP) pub_sym_1.o -o result

program.o: pub_sym_1.c
	$(CCOMP) -c pub_sym_1.c 

call:
	./result

clean:
	rm -rf *.o result