all: pr clean run
main.o: main.c
	gcc -std=c99 -c main.c
lab3.o: lab3.c
	gcc -std=c99 -c lab3.c
pr: main.o lab3.o
	gcc -std=c99 main.o lab3.o -g
clean:
	rm *.o
run: 
	./a.out kmbo22.dat abc.dat
