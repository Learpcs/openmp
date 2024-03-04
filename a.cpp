#include <iostream>
#include <omp.h>

void first_task() {

}

int main() {
	std::cout << "YEAR: " <<  _OPENMP / 100 << "\nMonth: " << _OPENMP % 100 << '\n';
}
