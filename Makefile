all: compile run

compile: main.c generate.c	
	gcc -g -o output main.c vector.c generate.c -lncurses

run:
	./output

clean:
	rm output

debug:
	gdb ./output



	#gcc -c generate.c generate.o 


