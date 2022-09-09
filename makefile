all: demo

run :
	./demo

demo: main.o
	gcc -o demo main.o -lncurses

main.o: main.c structures.h
	gcc -o main.o -c main.c -lncurses

clean:
	rm -rf demo *.o
