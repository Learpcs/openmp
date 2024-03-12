#include <iostream>
#include <omp.h>
#include <string>

long long sum = 0;

#pragma omp threadprivate (sum)

void first_task() {
	std::cout << "FIRST TASK\n";
	std::cout << "Is dynamic: " << omp_get_dynamic() << '\n';

	const int UP = int(1e3);

	#pragma omp parallel
	{
		for (int i = 0; i < UP; i += omp_get_thread_num() + 1) {
			sum += i;	
		}

		std::cout << omp_get_num_threads() << ' ' << sum << '\n';
	}

	std::cout << "OK" << std::endl;

	#pragma omp parallel
	{
		for (int i = 0; i < UP; i += omp_get_thread_num() + 1) {
			sum -= i;	
		}

		#pragma omp barrier

		std::cout << omp_get_thread_num() << ' ' << omp_get_num_threads() << ' ' << sum << '\n';
	}

}

void third_task() {
	std::cout << "THIRD TASK\n";
	int x = -1;
	#pragma omp parallel
	{
		#pragma omp critical
		{
			std::cout << "BEFORE: " << x << std::endl;
			x = omp_get_thread_num();
			std::cout << "AFTER: " << x << std::endl;
		}

	}
	std::cout << '\n';
}

#include <windows.h>

void fifth_task() {
	std::cout << "FIFTH TASK\n";

	#pragma omp parallel for schedule(static, 10) 	
	for (int i = 0; i < 21; i++)
	{
		Sleep(100);
		printf("Thread %d is running number %d\n", omp_get_thread_num(), i);
	}
}

int main() {
	std::cout << "Max threads: " << omp_get_max_threads() << std::endl << std::endl;
	first_task();
	third_task();
	fifth_task();

}
