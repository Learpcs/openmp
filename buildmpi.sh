#!/bin/bash
mpic++ $1.cpp -fopenmp -std=c++17 -O3 -march=native -o $1.out -Wall -Wextra -Wpedantic -Wshadow -Wconversion 
mpirun $1.out
