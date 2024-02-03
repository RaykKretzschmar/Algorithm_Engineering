#include <iostream>
#include <omp.h>
#include <random>
using namespace std;

double rnd(unsigned int *seed)
{
    *seed = (1140671485 * (*seed) + 12820163) % (1 << 24);
    return ((double)(*seed)) / (1 << 24);
}

int main()
{
    int n = 100000000;                 // number of points to generate
    int counter = 0;                   // counter for points lying in the first quadrant of a unit circle
    auto start_time = omp_get_wtime(); // omp_get_wtime() is an OpenMP library routine

// Parallel region
#pragma omp parallel
    {
        // Thread-private random number generators
        unsigned int seed = omp_get_thread_num();

// compute n points and test if they lie within the first quadrant of a unit circle
// Reduction for counter
#pragma omp for reduction(+ : counter)
        for (int i = 0; i < n; ++i)
        {
            auto x = rnd(&seed); // generate random number between 0.0 and 1.0
            auto y = rnd(&seed); // generate random number between 0.0 and 1.0
            if (x * x + y * y <= 1.0)
            { // if the point lies in the first quadrant of a unit circle
                ++counter;
            }
        }
    }

    auto run_time = omp_get_wtime() - start_time;
    auto pi = 4 * (double(counter) / n);

    cout << "pi: " << pi << endl;
    cout << "run_time: " << run_time << " s" << endl;
    cout << "n: " << n << endl;
}
