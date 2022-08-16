demo : premiersPas.o
	gcc -o demo premiersPas.o -lncurses

premiersPas.o : premiersPas.c
	gcc -c premiersPas.c

run : demo
	./demo

clean :
	rm -f demo *.o

help : sos
	chmod +x sos