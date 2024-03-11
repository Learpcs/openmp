#!/bin/bash
g++ $1.cpp -fopenmp -std=c++17 -O3 -march=native -o $1.out
./$1.out
