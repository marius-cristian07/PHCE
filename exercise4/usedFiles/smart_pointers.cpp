#include <iostream>
#include <memory>

#include "pico/stdlib.h"

int main()
{
    stdio_init_all();
    sleep_ms(2000);

    // Part 1: a raw pointer. You asked for the memory with new, so you must give it back.
    int* raw {new int {42}};
    std::cout << "*raw = " << *raw << std::endl;

    // TODO 1: give the memory back with delete, then set raw to nullptr.
    delete raw;
    raw = nullptr;

    std::cout << "raw is " << (raw ? "still set" : "nullptr") << std::endl;

    // Part 2: a smart pointer. It frees its memory by itself, so there is no delete.
    std::unique_ptr<int> first {std::make_unique<int>(42)};
    std::unique_ptr<int> second {nullptr};

    // TODO 2: move ownership from first to second
    second = std::move(first);

    std::cout << "first is " << (first ? "full" : "empty")
              << ", second is " << (second ? "full" : "empty") << std::endl;

    while (true)
    {
        sleep_ms(1000);
    }
}