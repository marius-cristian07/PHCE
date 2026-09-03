# Exercise Session 1

Work through the exercises in order. Write your answers directly in this file, in the
`_Answer:_` blocks under each question, and commit it together with any `.cpp` files
you are asked to attach, to your own copy of the repository.

---

## Exercise 1: Verify your setup

Confirm that the [`SETUP.md`](../SETUP.md)
instructions have been followed and your Pico development environment is in place.

**Anything that did not work, and how you solved it:**

> _Answer:_
> everything worked

---

## Exercise 2: Build and run the blinker

Create, build and flash the example **Blink** project to confirm that your development
environment works from source code all the way to running firmware on the Pico W.

### Instructions

1. Open **Visual Studio Code**.
2. Open the **Raspberry Pi Pico** extension from the sidebar.
3. Create a new project and select the **Blink** example.
   - Select **Pico W** as the target board.
   - Choose a folder for the project.
   - Create/open the project in VS Code.
4. Add a `printf("Test");` statement inside the while loop in main().
5. Build the project.
   - Use the **Compile / Build** command from the Raspberry Pi Pico extension.
   - Watch the terminal output and make sure the build finishes without errors.
6. Connect the Pico W to your computer using USB.
7. Flash the program to the Pico W using the **Run / Flash** command in the
   Raspberry Pi Pico extension or by manually uploading the .uf2 file.
8. Check the board. The LED should now blink continuously.
9. Connect the **Debug Probe** (hardware) and open the **Serial Monitor**, then look for the output.

### Checklist
- [x] Blink project created and opened
- [x] Project builds without errors
- [x] Program flashed to the Pico W
- [x] The LED is blinking
- [x] Serial Monitor output is visible

**If the build or flash failed, what was the error and what fixed it?**

> _Answer:_
> the build and flash worked, the serial monitor didnt show initially the print because the sdk wasnt initialized so we added a the beginning of main the line "stdio_init_all();" and recompiled and flashed so now its all good

---

## Exercise 3: Explore PicoBricks

For each hardware module on the PicoBricks board, answer the following
questions:

1. What is the name of this module?
2. What does this module do?
3. Is it an input or output?

### Module 1

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | |
| 2 | Purpose | |
| 3 | Input or output | |

_Add more blocks as you write._


### Module 1

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | OLED Screen |
| 2 | Purpose | Displays visual data, text, or graphics |
| 3 | Input or output | Output |

### Module 2

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | RGB LED |
| 2 | Purpose | Emits light in various customizable colors |
| 3 | Input or output | Output |

### Module 3

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Button & LED |
| 2 | Purpose | Detects physical presses (button) and provides visual indication (LED) |
| 3 | Input or output | Input and Output |

### Module 4

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Temp. Humidity Sensor |
| 2 | Purpose | Measures the ambient temperature and relative humidity |
| 3 | Input or output | Input |

### Module 5

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Electronic Switch (Relay) |
| 2 | Purpose | Opens or closes electrical circuits to control higher-power devices |
| 3 | Input or output | Output |

### Module 6

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Motor Driver |
| 2 | Purpose | Controls the direction and speed of connected motors |
| 3 | Input or output | Output |

### Module 7

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Infrared and Bluetooth Sensor |
| 2 | Purpose | Receives IR signals and provides wireless Bluetooth communication |
| 3 | Input or output | Input / Output |

### Module 8

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Speaker (Buzzer) |
| 2 | Purpose | Emits simple tones, beeps, or alarms |
| 3 | Input or output | Output |

### Module 9

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Light Sensor (LDR) |
| 2 | Purpose | Detects the intensity of ambient light |
| 3 | Input or output | Input |

### Module 10

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | Rotatable Sensor (Potentiometer) |
| 2 | Purpose | Provides variable readings based on the position of a rotating dial |
| 3 | Input or output | Input |

### Module 11

| # | Question | Answer |
|---|----------|--------|
| 1 | Name | ProtoBoard |
| 2 | Purpose | Provides a blank perforated area for soldering or wiring custom circuits and components |
| 3 | Input or output | N/A (Passive prototyping area) |

---

## Exercise 4: Morse Code

Make a program that blinks `help!` as Morse
code using the LED abstractions in the blink project (`pico_set_led`). How you represent
and translate Morse code is up to you.

Rules:

- dot = 1 time unit (200ms)
- dash = 3 time units
- gap between dot/dash elements within one letter = 1 unit
- gap between letters = 3 units
- gap between words = 7 units

Use the Morse-code reference below and the C++ book/documentation, Google as needed.

Reference: <https://en.wikipedia.org/wiki/Morse_code>

Simply copy the modified .c file(s) containing your solution to the exercise folder.

**Attached file(s):**

> _Filename:_ blink.c (but its a modified version of it we just used the template for a quick start)
>
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
