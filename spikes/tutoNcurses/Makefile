demo2 : ajoutdeCouleurs.o
	gcc -o demo2 ajoutdeCouleurs.o -lncurses

ajoutdeCouleurs.o :
	gcc -c ajoutdeCouleurs.c

demo1 : premiersPas.o
	gcc -o demo1 premiersPas.o -lncurses

premiersPas.o : premiersPas.c
	gcc -c premiersPas.c

run : demo2
	./demo2

clean :
	rm -f demo1 demo2 *.o

help : sos
	chmod +x sos