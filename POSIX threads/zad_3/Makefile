CCOMP = gcc

all: clean result call

result: threads3.o
	$(CCOMP) threads3.o -o result -lm

threads2.o: threads3.c
	$(CCOMP) -c threads3.c

call:
	./result

clean:
	rm -f *.o result
	
