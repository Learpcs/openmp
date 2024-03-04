#include <iostream>
#include <omp.h>

void first_task() {
	std::cout << "YEAR: " <<  _OPENMP / 100 << "\nMonth: " << _OPENMP % 100 << '\n';
}

void third_task() {
	const char *message = "Hello world";
	#pragma omp parallel
	{
		#pragma omp single
		{
			std::cout << "Amount of threads: " << omp_get_num_threads() << std::endl;
		}

		#pragma omp barrier

		std::cout << message << std::endl;
	}
}

int main() {
	first_task();
	third_task();
}
