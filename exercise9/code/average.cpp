#include <cstdint>
#include <iostream>

#include "pico/stdlib.h"

constexpr std::uint32_t REPEAT_MS {5000};
constexpr double COUNT {3.0};  // how many values average() takes

// TODO 1: a function template average(). It takes three values of the same type T and
//         returns their average as a double. Divide by COUNT.

int main()
{
    stdio_init_all();

    while (true)
    {
        // TODO 2: print the average of the ints 2, 4 and 7, and of the doubles 2.5, 4.5
        //         and 6.5.

        // TODO 3: print the average of 1, 2.5 and 3 (step 5).

        std::cout << std::endl;
        sleep_ms(REPEAT_MS);
    }
}
