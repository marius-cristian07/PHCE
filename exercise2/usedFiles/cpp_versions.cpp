/*
 * C11 blinker starter for Exercise Session 2, Exercise 1.
 *
 * Copy this file into the generated .c file of a project created with the
 * Raspberry Pi Pico VS Code extension (C version, i.e. "Generate C++ code"
 * NOT ticked). See exercise2/instructions.md for the full exercise.
 *
 * The LED is the PicoBricks LED on GPIO 7. The Pico W's own tiny LED sits
 * behind the WiFi chip: driving it links in a large driver and makes the
 * firmware about 20 times bigger. That is why this exercise does not use it.
 *
 * Exercise 1 asks you to rewrite this file in C++17. The constructs to
 * convert are marked below.
 */
#include <stdio.h>
#include "pico/stdlib.h"

/* #define = text substitution. Before the compiler even runs, every LED_PIN
 * in the file is replaced by the number 7. */
constexpr uint led_pin = 7;
constexpr uint blink_delay_ms = 250;

/* enum = names for integers. LED_OFF is 0 and LED_ON is 1, nothing more.
 * The compiler will happily pass them anywhere a number is expected. */
enum class LedState { Off = false, On = true };

/* struct = a small bundle of data that belongs together. A blinker needs to
 * know two things: which pin its LED is on, and how fast to blink.
 * (uint is the SDK's shorthand for unsigned int, a 32-bit number that
 * cannot be negative.) */
struct blinker {
    uint pin;      // which GPIO pin the LED is on
    uint delay_ms; // how long each on/off phase lasts
};

static void waste_time() {
    for (uint32_t i = 0; i < 1000000; ++i) {
        // do nothing
    }
}
/* A free function: it takes a pointer to a blinker and does one full blink
 * (on, wait, off, wait). static just means "used only in this file". */
static void blink_once(struct blinker* b) {
    gpio_put(b->pin, static_cast<bool>(LedState::On));
    sleep_ms(b->delay_ms);

    waste_time();

    gpio_put(b->pin, static_cast<bool>(LedState::Off));
    sleep_ms(b->delay_ms);
}

int main() {
    stdio_init_all(); // start printf/puts output (Serial Monitor over the Debug Probe)

    // Make the PicoBricks LED pin an output.
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Create one blinker and tell it which pin to use and how fast to blink.
    struct blinker b = { .pin = led_pin, .delay_ms = blink_delay_ms };
    puts("C11 blinker started");

    while (true) { // runs forever, like every embedded program
        blink_once(&b);
    }
    return 0;
}
