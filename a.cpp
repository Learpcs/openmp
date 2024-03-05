#include <iostream>
#include <omp.h>

void first_task() {
	std::cout << "FIRST TASK\n";
	std::cout << "YEAR: " <<  _OPENMP / 100 << "\nMonth: " << _OPENMP % 100 << '\n';
	std::cout << "\n";
}

void third_task() {
	std::cout << "THIRD TASK\n";
	#pragma omp parallel
	{
		#pragma omp single
		{
			std::cout << "Amount of threads: " << omp_get_num_threads() << std::endl;
		}

		#pragma omp barrier

		std::cout << "Hello world" << std::endl;
	}
	std::cout << '\n';
}

void fifth_task() {
	std::cout << "FIFTH TASK\n";
	#pragma omp parallel num_threads(1 << 14)
	{
		#pragma omp single
		{
			std::cout << "Amount of threads: " << omp_get_num_threads() << std::endl;
		}

		#pragma omp barrier

		//std::cout << "Hello world" << std::endl;
	}
	std::cout << '\n';
}

int main() {
	std::cout << "Max threads: " << omp_get_max_threads() << std::endl << std::endl;
	first_task();
	third_task();
	fifth_task();

}
