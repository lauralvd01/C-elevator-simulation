all: demo

run :
	./demo

demo: main.o
	gcc -o demo obj/main.o -lncurses

main.o: main.c structures.h
	gcc -o obj/main.o -c main.c -lncurses

clean:
	rm -rf demo obj/*.o
