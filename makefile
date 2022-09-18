all: demo

run :
	./demo

demo: main.o display.o tools.o liste.o immeuble.o
	gcc -o demo obj/main.o obj/display.o obj/liste.o obj/tools.o obj/immeuble.o -lncurses

tools.o: tools.c structures.h
	gcc -o obj/tools.o -c tools.c -lncurses

liste.o: liste.c structures.h
	gcc -o obj/liste.o -c liste.c -lncurses

immeuble.o: immeuble.c structures.h
	gcc -o obj/immeuble.o -c immeuble.c -lncurses

display.o: display.c structures.h
	gcc -o obj/display.o -c display.c -lncurses

main.o: main.c structures.h
	gcc -o obj/main.o -c main.c -lncurses

clean:
	rm -rf ./demo obj/*.o
