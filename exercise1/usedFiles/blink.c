/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define UNIT_MS 200

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

void dot(void) {
    pico_set_led(true);
    sleep_ms(UNIT_MS);     // 1 unit ON
    pico_set_led(false);
    sleep_ms(UNIT_MS);     // 1 unit gap between elements
}

void dash(void) {
    pico_set_led(true);
    sleep_ms(3 * UNIT_MS); // 3 units ON
    pico_set_led(false);
    sleep_ms(UNIT_MS);     // 1 unit gap between elements
}

void letter_gap(void) {
    // 3 units total gap between letters.
    // Since dot()/dash() ends with 1 unit OFF, we wait 2 additional units here.
    sleep_ms(2 * UNIT_MS);
}

void word_gap(void) {
    // 7 units total gap between words.
    // Since dot()/dash() ends with 1 unit OFF, we wait 6 additional units here.
    sleep_ms(6 * UNIT_MS);
}

int main() {
    stdio_init_all();

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    
    while (true) {
        printf("Blinking 'help!' in Morse code...\n");

        // 'h' : . . . .
        dot(); dot(); dot(); dot();
        letter_gap();

        // 'e' : .
        dot();
        letter_gap();

        // 'l' : . - . .
        dot(); dash(); dot(); dot();
        letter_gap();

        // 'p' : . - - .
        dot(); dash(); dash(); dot();
        letter_gap();

        // '!' : - . - . - -
        dash(); dot(); dash(); dot(); dash(); dash();
        
        // Wait 7 units before restarting the sequence
        word_gap();
    }
}
