all: compile run

compile: main.c generate.c	
	gcc -g -o output main.c generate.c -lncurses

run:
	./output

clean:
	rm output


	#gcc -c generate.c generate.o 


