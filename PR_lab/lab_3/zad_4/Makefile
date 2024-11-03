CCOMP = gcc

all: clean result call

result: threads4.o
	$(CCOMP) threads4.o -o result -g

threads4.o: threads4.c
	$(CCOMP) -c threads4.c -g

call:
	sudo ./result

clean:
	rm -f *.o result
	
