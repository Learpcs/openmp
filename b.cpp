#include <iostream>
#include <omp.h>
#include <string>

void first_task() {
	std::cout << "FIRST TASK\n";
	auto f = []() {
		if (omp_in_parallel()) {
			std::cout << "1" << std::endl;
		}
		else {
			std::cout << "2" << std::endl;
		}
	};
	#pragma omp parallel
	{
		#pragma omp single
			f();
	}
	f();
}

void third_task() {
	std::cout << "THIRD TASK\n";
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			std::cout << "Amount of threads: " << omp_get_num_threads() << std::endl;
		}

		std::cout << omp_get_thread_num() << std::endl;

	}
	std::cout << '\n';
}

void fifth_task() {
	std::cout << "FIFTH TASK\n";
	int x = 0;

	#pragma omp parallel private(x) 
	{
		#pragma omp master
		if (!omp_get_thread_num()) {
			++x;
		}

		#pragma omp barrier

		std::string inp = std::to_string(omp_get_thread_num()) + " " + std::to_string(x) + "\n";
		std::cout << inp;
	}
	std::cout << '\n';
}

int main() {
	std::cout << "Max threads: " << omp_get_max_threads() << std::endl << std::endl;
	first_task();
	third_task();
	fifth_task();

}
