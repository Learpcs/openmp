#!/bin/bash
g++ $1.cpp -fopenmp -std=c++17 -O3 -march=native -o $1 -Wall -Wextra -Wpedantic -Wshadow -Wconversion 
./$1.exe
