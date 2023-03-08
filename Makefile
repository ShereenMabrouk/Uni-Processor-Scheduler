cc= gcc -g
CC= g++ -g
make:
	$(cc) -c main.cc
	$(CC) -o lab4 main.o
