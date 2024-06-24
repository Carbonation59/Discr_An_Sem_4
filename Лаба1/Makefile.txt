
CC=g++
FLAGS=-std=c++14 -pedantic -Wall -Wextra -Wno-unused-variable
OUTPUT=lab1

all: lab1.cpp
        $(CC) $(FLAGS) lab1.cpp -o $(OUTPUT)
generator: generator.cpp
        $(CC) $(FLAGS) generator.cpp -o generator
benchmark: benchmark.cpp
        $(CC) $(FLAGS) benchmark.cpp -o benchmark
clean:
        rm *.o $(OUTPUT)
