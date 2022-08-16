demo : main.o
	gcc -o demo main.o -lncurses

main.o : main.c
	gcc -c main.c

run : demo
	./demo

clean :
	rm -f demo *.o

help : sos
	chmod +x sos