CCOMP = gcc

all: clean result call

result: pthreads_detach_kill.o
	$(CCOMP) pthreads_detach_kill.o -o result

pthreads_detach_kill.o: pthreads_detach_kill.c
	$(CCOMP) -c pthreads_detach_kill.c

call:
	./result

clean:
	rm -f *.o result