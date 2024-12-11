CCOMP = javac

all: compile run

compile: Histogram_test.class Obraz.class Watek.class Watek2.class Watek3.class Watek4.class Watek5.class

Histogram_test.class: Histogram_test.java Obraz.class Watek.class Watek2.class Watek3.class Watek4.class Watek5.class
	$(CCOMP) Histogram_test.java

Obraz.class: Obraz.java
	$(CCOMP) Obraz.java

Watek.class: Watek.java
	$(CCOMP) Watek.java

Watek2.class: Watek2.java
	$(CCOMP) Watek2.java

Watek3.class: Watek3.java
	$(CCOMP) Watek3.java

Watek4.class: Watek4.java
	$(CCOMP) Watek4.java

Watek5.class: Watek5.java
	$(CCOMP) Watek5.java


run:
	java Histogram_test

clean:
	rm -r *.class