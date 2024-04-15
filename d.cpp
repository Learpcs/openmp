#include <iostream>
#include <omp.h>
#include <string>
#include <Windows.h>

long long sum = 0;

void first_task() {
	std::cout << "FIRST TASK\n";
	 #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        std::cout << "Message from thread " << thread_id << " before barrier" << std::endl;

        #pragma omp barrier

        std::cout << "Message from thread " << thread_id << " after barrier" << std::endl;
    }
}

void third_task() {
	std::cout << "THIRD TASK\n";
	int global_var;
	#pragma omp parallel private(global_var)
    {
        int thread_id = omp_get_thread_num();

        #pragma omp critical
        {
            global_var = thread_id;  // Critical section to update the global variable
            std::cout << "Thread " << thread_id << " set global_var to " << global_var << std::endl;
        }
    }
}

#include <windows.h>

void fifth_task() {

	omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        omp_set_lock(&lock);
        // Critical section starts
        std::cout << "Thread " << thread_id << " is starting its work" << std::endl;
        Sleep(100); // Simulate some work
        std::cout << "Thread " << thread_id << " has finished its work" << std::endl;
        // Critical section ends
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);
}

int main() {
	std::cout << "Max threads: " << omp_get_max_threads() << std::endl << std::endl;
	first_task();
	third_task();
	fifth_task();

}
