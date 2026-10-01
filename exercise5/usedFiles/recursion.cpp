#include <iostream>

#include "pico/stdlib.h"

// Prints n, n - 1, ... 1, then "Liftoff!". Then each call says when it ends.
void countdown(int n)
{
    if (n == 0)  // the base case: stop here
    {
        std::cout << "Liftoff!" << std::endl;
        return;
    }
    std::cout << n << std::endl;
    countdown(n - 1);  // the function calls itself
    std::cout << "back in " << n << std::endl;
}

// TODO 1 & 2: Adds 1 + 2 + ... + n recursively.
int sum_to(int n)
{
    if (n == 0)
    {
        return 0;
    }
    return n + sum_to(n - 1);
}

// Step 4: Calculates n! (1 * 2 * ... * n) recursively. factorial(0) is 1.
int factorial(int n)
{
    if (n == 0)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

int main()
{
    stdio_init_all();
    sleep_ms(5000);  // 4s delay so Serial Monitor has time to attach

    std::cout << "--- countdown(5) ---" << std::endl;
    countdown(5);

    std::cout << "\n--- Calculations ---" << std::endl;
    std::cout << "sum_to(10) = " << sum_to(10) << std::endl;        // Step 3
    std::cout << "factorial(5) = " << factorial(5) << std::endl;    // Step 5

    while (true)
    {
        sleep_ms(1000);
    }
}