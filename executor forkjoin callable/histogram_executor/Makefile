CCOMP = javac

all: compile run

compile: Histogram_test.class Obraz.class Watek3.class

Histogram_test.class: Histogram_test.java Obraz.class Watek3.class
	$(CCOMP) Histogram_test.java

Obraz.class: Obraz.java
	$(CCOMP) Obraz.java

Watek3.class: Watek3.java
	$(CCOMP) Watek3.java

run:
	java Histogram_test

clean:
	rm -r *.class