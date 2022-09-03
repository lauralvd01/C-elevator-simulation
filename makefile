all: demo

run :
	./demo

demo: create.o automate.o display.o main.o
	gcc -o demo create.o automate.o display.o main.o -lncurses

create.o: create.c structures.h
	gcc -o create.o -c create.c -W -Wall -Wconversion -Werror -pedantic

automate.o: automate.c structures.h
	gcc -o automate.o -c automate.c -W -Wall -Wconversion -Werror -pedantic

display.o: display.c structures.h
	gcc -o display.o -c display.c -W -Wall -Wconversion -Werror -pedantic -lncurses

main.o: main.c structures.h
	gcc -o main.o -c main.c -W -Wall -Wconversion -Werror -pedantic -lncurses

clean:
	rm -rf demo *.o

