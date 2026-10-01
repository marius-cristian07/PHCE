#include <iostream>

#include "pico/stdlib.h"

void swap_by_value(int a, int b)  // gets copies
{
    const int temp {a};
    a = b;
    b = temp;
}

void swap_by_pointer(int* a, int* b)  // gets addresses
{
    // TODO 1: dereference pointers to swap the actual values stored at the addresses
    const int temp {*a};
    *a = *b;
    *b = temp;
}

void swap_by_reference(int& a, int& b)  // gets aliases/references
{
    // TODO 2: syntax is identical to swap_by_value, but modifies original variables
    const int temp {a};
    a = b;
    b = temp;
}

int main()
{
    stdio_init_all();
    sleep_ms(5000);  // time to open the Serial Monitor

    int x {1};
    int y {2};
    std::cout << "start:         x = " << x << ", y = " << y << std::endl;

    swap_by_value(x, y);
    std::cout << "by value:     x = " << x << ", y = " << y << std::endl;

    swap_by_pointer(&x, &y);
    std::cout << "by pointer:   x = " << x << ", y = " << y << std::endl;

    swap_by_reference(x, y);
    std::cout << "by reference: x = " << x << ", y = " << y << std::endl;

    while (true)
    {
        sleep_ms(1000);
    }
}