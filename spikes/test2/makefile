all: demo

run :
	./demo

demo: create.o automate.o print.o main.o
	gcc -o demo create.o automate.o print.o main.o

create.o: create.c structures.h
	gcc -o create.o -c create.c -W -Wall -Wconversion -Werror -pedantic

automate.o: automate.c structures.h
	gcc -o automate.o -c automate.c -W -Wall -Wconversion -Werror -pedantic

print.o: print.c structures.h
	gcc -o print.o -c print.c -W -Wall -Wconversion -Werror -pedantic

main.o: main.c structures.h
	gcc -o main.o -c main.c -W -Wall -Wconversion -Werror -pedantic

clean:
	rm -rf *.o

mrproper: clean
	rm -rf demo
