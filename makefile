all: demo

run :
	./demo

demo: create.o automate.o display.o main.o
	gcc -o demo obj/create.o obj/automate.o obj/display.o obj/main.o -lncurses

create.o: create.c structures.h
	gcc -o obj/create.o -c create.c 

automate.o: automate.c structures.h
	gcc -o obj/automate.o -c automate.c 

display.o: display.c structures.h
	gcc -o obj/display.o -c display.c -lncurses

main.o: main.c structures.h
	gcc -o obj/main.o -c main.c -lncurses

clean:
	rm -rf demo obj/*.o
