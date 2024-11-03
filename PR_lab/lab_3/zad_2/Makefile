CCOMP = gcc

all: clean result call

result: threads2.o
	$(CCOMP) threads2.o -o result

threads2.o: threads2.c
	$(CCOMP) -c threads2.c

call:
	./result

clean:
	rm -f *.o result