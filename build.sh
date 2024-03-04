#!/bin/bash
g++ $1.cpp -fopenmp -std=c++23 -O3 -march=native
./$1.out
