#include <iostream>
#include <chrono>
#include <omp.h>

using f_type = void(*)();

const int N = int(1e9);

void lol() {
    omp_set_dynamic(0);

    #pragma 
    {
        long long sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < N; ++i) {
            sum += i;
        }
        std::cout << sum << ' ' << N * 1ll * (N - 1) / 2 << '\n';

    }

}

void benchmark(f_type f) {
    using namespace std::chrono;
    using namespace std;

    steady_clock clk;
    auto prev = clk.now();
    f();
    auto now = clk.now();
    cout << duration_cast<milliseconds>(now - prev).count() << '\n';
}

int main()
{
    benchmark(lol);
}

