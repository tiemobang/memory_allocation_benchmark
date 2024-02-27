CC = g++
CFLAGS = -Wall -g -fopenmp -std=c++11 -O3

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp

clean:
	rm -f main

pagefaults: clean main
	OMP_PROC_BIND=spread ./main 0
	OMP_PROC_BIND=spread ./main 1

numa: clean main
	OMP_PROC_BIND=spread ./main 2
	OMP_PROC_BIND=spread ./main 3