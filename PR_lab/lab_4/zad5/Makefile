CCOMP = gcc

all: program call


program: pub_sym_2.o
	$(CCOMP) pub_sym_2.o -o result

program.o: pub_sym_2.c
	$(CCOMP) -c pub_sym_2.c 

call:
	./result

clean:
	rm -rf *.o result