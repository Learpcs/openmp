#include <iostream>
#include <omp.h>
#include <string>
#include <Windows.h>
#include <cmath>
#include <chrono>

long long sum = 0;

const int N = 1000;
const float h = 1.0f / N;

float f(float x, float y, float z) {
    return 1 / (2 + sinf(x * x + y * y + z * z));
}

void first_task() {
    std::cout << "FIRST TASK" << std::endl;
    float ans = 0;
    #pragma omp parallel for reduction(+:ans)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                ans += f(h * (i + 1.0f / 2.0f), h * (j + 1.0f / 2.0f), h * (k + 1.0f / 2.0f));
            }
        }
    }

    std::cout << ans / h / h / h << std::endl;
}

void second_task() {
    std::cout << "SECOND TASK" << std::endl;
	float ans = 0;
    #pragma omp parallel for reduction(+:ans) schedule(dynamic)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                ans += f(h * (i + 1.0f / 2.0f), h * (j + 1.0f / 2.0f), h * (k + 1.0f / 2.0f));
            }
        }
    }

    std::cout << ans / h / h / h << std::endl;
}

void third_task() {
    std::cout << "THIRD TASK" << std::endl;
	float ans = 0;
    #pragma omp parallel for reduction(+:ans) schedule(static)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                ans += f(h * (i + 1.0f / 2.0f), h * (j + 1.0f / 2.0f), h * (k + 1.0f / 2.0f));
            }
        }
    }

    std::cout << ans / h / h / h << std::endl;
}

void fourth_task() {
    std::cout << "FOURTH TASK" << std::endl;
    float ans = 0;
    #pragma omp parallel for reduction(+:ans)
    for (int i = 0; i < N; ++i) {
        for (int j = -(N - i); j < N; ++j) {
            for (int k = -sqrtf((N - i) * (N - i) - j * j); k <= sqrtf((N - i) * (N - i) - j * j); ++k) {
                ans += f(h * (i + 1.0f / 2.0f), h * (j + 1.0f / 2.0f), h * (k + 1.0f / 2.0f));
            }
        }
    }

    std::cout << ans / h / h / h << std::endl;
}

void fifth_task() {
    std::cout << "FIFTH TASK" << std::endl;
	float ans = 0;
    #pragma omp parallel for reduction(+:ans) schedule(dynamic)
    for (int i = 0; i < N; ++i) {
        for (int j = -(N - i); j < N; ++j) {
            for (int k = -sqrtf((N - i) * (N - i) - j * j); k <= sqrtf((N - i) * (N - i) - j * j); ++k) {
                ans += f(h * (i + 1.0f / 2.0f), h * (j + 1.0f / 2.0f), h * (k + 1.0f / 2.0f));
            }
        }
    }

    std::cout << ans / h / h / h << std::endl;
}

void sixth_task() {
    std::cout << "SIXTH TASK" << std::endl;
	float ans = 0;
    #pragma omp parallel for reduction(+:ans) schedule(static)
    for (int i = 0; i < N; ++i) {
        for (int j = -(N - i); j < N; ++j) {
            for (int k = -sqrtf((N - i) * (N - i) - j * j); k <= sqrtf((N - i) * (N - i) - j * j); ++k) {
                ans += f(h * (i + 1.0f / 2.0f), h * (j + 1.0f / 2.0f), h * (k + 1.0f / 2.0f));
            }
        }
    }

    std::cout << ans / h / h / h << std::endl;
}

void measure_time(void (*f)()) {
    auto start = std::chrono::steady_clock::now();

    f();

    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Function execution took " << duration.count() << " milliseconds." << std::endl;
}

int main() {
    void (*arr[6])() = {first_task, second_task, third_task, fourth_task, fifth_task, sixth_task};
    for (int i = 0; i < 6; ++i) {
        measure_time(arr[i]);
    }
}
