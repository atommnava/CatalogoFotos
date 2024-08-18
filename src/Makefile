ejecutable.out: catalogoFotos.o funFotos.o
	gcc -o catalogoFotos.exe catalogoFotos.o funFotos.o

catalogoFotos.o: catalogoFotos.c
	gcc -c catalogoFotos.c

funFotos.o: funFotos.c
	gcc -c funFotos.c

clean:
	rm catalogoFotos.exe
	rm *.o