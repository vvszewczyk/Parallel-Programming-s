CCOMP = javac

all: compile run

compile: Calka_callable.class Calka_runnable.class Simple_executor_test.class ResultManager.class

Calka_callable.class: Calka_callable.java
	$(CCOMP) Calka_callable.java

Calka_runnable.class: Calka_runnable.java
	$(CCOMP) Calka_runnable.java

Simple_executor_test.class: Simple_executor_test.java
	$(CCOMP) Simple_executor_test.java

scal_tab.class: scal_tab.java
	$(CCOMP) scal_tab.java

ResultManager.class: ResultManager.java
	$(CCOMP) ResultManager.java

run:
	java Simple_executor_test

clean:
	rm -f *.class
