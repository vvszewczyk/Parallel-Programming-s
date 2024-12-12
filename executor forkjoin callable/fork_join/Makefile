CCOMP = javac

all: compile run

compile: ForkJoinPoolSort.class scal_tab

ForkJoinPoolSort.class: ForkJoinPoolSort.java
	$(CCOMP) ForkJoinPoolSort.java

scal_tab: 
	$(CCOMP) scal_tab.java

run:
	java ForkJoinPoolSort

clean:
	rm -f *.class
