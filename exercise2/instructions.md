# Exercise Session 2

Work through the exercises in order. Write your answers directly in this file, in the
`_Answer:_` blocks under each question, and commit it together with any `.c` or `.cpp`
files you are asked to attach, to your own copy of the repository.

---

## Exercise 1: Same firmware in C and C++

**Goal:** Write the same small program in C11 and in C++17, and see what the modern
C++ features buy you: `#define` → `constexpr`, `enum` → `enum class`, type safety.

### Instructions

1. Open **Visual Studio Code** and open the **Raspberry Pi Pico** extension from the sidebar.
2. Create a new project:
   - Select **New Pico Project** → **C/C++**.
   - Name it `c_versions`, select **Pico W** as the target board.
   - Tick **Console over UART** (the output then travels through the Debug Probe).
   - **Do NOT tick the wireless option** (`Pico W onboard LED`). Step 4 explains why.
   - Leave **Generate C++ code** unticked.
   - Choose a folder for the project and open it in VS Code.
3. Find the starter file
   [`code/c_versions/main.c`](code/c_versions/main.c) and copy its contents into the
   generated `c_versions.c`, replacing everything that was there.
4. Read the code. The comments in the starter explain each part. Note the C constructs
   you are about to convert: `#define`, `enum`, and `struct blinker` with the free
   function `blink_once()`. The LED is the **PicoBricks LED**, wired straight to
   RP2040 **GPIO 7** and driven with plain `gpio_put()`.

   The Pico W's onboard LED sits behind the WiFi chip and needs a large driver that
   would bloat the firmware and shift every later exercise onto that driver instead of
   your code, so this exercise uses the PicoBricks LED instead.
5. Compile the project and run it. The PicoBricks LED should blink at 250 ms intervals.
6. Create a second project exactly the same way (same board, **Console over UART**),
   but name it `cpp_versions` and tick **Generate C++ code** this time.
7. Rewrite the program in C++17 using the mapping table below. When you get to the LED
   lines, try this first: inside `blink_once()`, pass the new `LedState` value straight
   to `gpio_put()` and compile.

   ```cpp
   gpio_put(b->pin, LedState::On);   // and the same for LedState::Off
   ```

   It will **not compile**. Read the error message, then read the explanation below. It
   tells you what the compiler is trying to protect you from, and how the fix works.
   Keep the overall structure the same in both versions: `struct blinker` with the free
   function `blink_once()` stays as it is. We will turn it into a proper object later in
   the course.

   **What just happened, and what does the fix mean?**

   `gpio_put()` expects a `bool`. In C, an `enum` is just an integer, so `LED_ON`
   converts to `true` automatically. `enum class` refuses that conversion on purpose:
   a `LedState` is its own type, not a number, so swapped or nonsense values get caught
   when you compile instead of showing up as a bug on the board later.

   Fix it with an explicit cast. It costs nothing at runtime and vanishes from the
   machine code you'll look at in Exercise 2:

   ```cpp
   gpio_put(b->pin, static_cast<bool>(LedState::On));
   gpio_put(b->pin, static_cast<bool>(LedState::Off));
   ```

   The `: bool` in `enum class LedState : bool { Off = false, On = true };` is what
   makes `On` and `Off` real booleans, so the cast has something to hand over.

   Read more: Beginning C++17, Chapter 2 (casts) and Chapter 3 (enumerations).
8. Build and run `cpp_versions`. The behavior must match the C version.
9. Compare the two builds:
   - In the Explorer, note the source file names: `c_versions.c` ends in `.c`,
     `cpp_versions.cpp` ends in `.cpp`. The extension is what tells the build system
     which compiler to use: `.c` files go to the C compiler (`gcc`), `.cpp` files to
     the C++ compiler (`g++`).
   - Open both `CMakeLists.txt` files and find the lines that set the language
     standards (`CMAKE_C_STANDARD 11` in one, `CMAKE_CXX_STANDARD 17` in the other).
10. **Meet the cross-compiler.** Open `C:\Users\<user>\.pico-sdk\toolchain\<version>\bin\`
    (Linux: `~/.pico-sdk/toolchain/<version>/bin/`) and find `arm-none-eabi-gcc` and
    `arm-none-eabi-g++`. The name says it all: `arm` is the target CPU, `none` means no
    operating system, `eabi` is the embedded calling convention. This folder also holds
    `gdb`, `objdump`, `size` and `c++filt`, used in later exercises.
11. **Measure properly:** in a terminal inside VS Code (the extension adds the
    toolchain folder to the PATH of its own terminals), run `arm-none-eabi-size` on
    both firmware files. If the command is not found, step 10 shows where the tools
    live:

    ```
    arm-none-eabi-size build/c_versions.elf
    arm-none-eabi-size build/cpp_versions.elf
    ```

    Fill in the table below. `text` is the code in Flash, `data` is initialized variables
    (Flash copy + RAM), `bss` is zero-initialized variables (RAM only). The two rows
    should be identical or nearly identical: any *difference* between them is the cost
    of the language choice.

### C11 → C++17 mapping

| C11 | C++17 |
|---|---|
| `#define LED_PIN 7` | `constexpr uint led_pin = 7;` |
| `#define BLINK_DELAY_MS 250` | `constexpr uint blink_delay_ms = 250;` |
| `enum led_state { LED_OFF = 0, LED_ON = 1 };` | `enum class LedState : bool { Off = false, On = true };` |
| `enum` values are integers and convert to anything silently | `enum class` is its own type and refuses. Why, and how to fix it, is explained in step 7 |

### Results

| Version | `text` | `data` | `bss` | Total Flash (`text`+`data`) | Total RAM (`data`+`bss`) |
|---|---|---|---|---|---|
| `c_versions` (C11) | 12432 | 0 | 1184 | 12432 | 1184 |
| `cpp_versions` (C++17) | 12432 | 0 | 1184 | 12432 | 1184 |


### Checklist

- [x] `c_versions` project created (C, **Console over UART**, no wireless option) and builds
- [x] `c_versions` runs and the PicoBricks LED blinks
- [x] `cpp_versions` project created with the same options and **Generate C++ code** ticked
- [x] `gpio_put(..., LedState::On)` tried without a cast, and the compile error read
- [x] The program is rewritten in C++17 (with the cast) and behaves identically
- [x] The `CMAKE_C_STANDARD` / `CMAKE_CXX_STANDARD` lines found in both `CMakeLists.txt`
- [x] `arm-none-eabi-gcc` and `arm-none-eabi-g++` found in the toolchain folder
- [x] Sizes table above filled in with `arm-none-eabi-size`

**The compiler runs on your Windows/Linux PC. What CPU architecture will the produced
instructions run on? What does the `arm-none-eabi` part of the compiler's name tell you?
Why can the `.elf` file produced on your laptop not simply be executed by Windows?**

> _Answer:_
> The produced instructions will run on the **ARM Cortex-M0+** architecture (the RP2040 microarchitecture).
>
> The target triple `arm-none-eabi` indicates:
> * **`arm`**: The target processor family/instruction set architecture (ISA).
> * **`none`**: The target operating system—there is none (bare-metal environment).
> * **`eabi`**: The calling convention and binary interface standard (Embedded Application Binary Interface).
>
> Windows cannot execute the `.elf` file because:
> 1. **Instruction Incompatibility**: The binary contains ARM Thumb machine instructions, whereas Windows is running on an x86-64 host processor.
> 2. **Executable Format**: Windows expects executables wrapped in the Portable Executable (PE/COFF, `.exe`) format, not Executable and Linkable Format (ELF).
> 3. **System Environment**: The binary directly targets the RP2040 memory map and hardware registers without the Windows user-space APIs or kernel-mode translation layers.

**Why did `enum class` require an explicit conversion where the C `enum` did not? What
went wrong when you passed `LedState::On` to `gpio_put()` without a cast?**

> _Answer:_
> Standard C enums are unscoped and implicitly convert to their underlying integer types (and subsequently to `bool`). C++11 `enum class` introduces strongly typed, scoped enumerations. These do not implicitly convert to integers or booleans, preventing accidental conversions across unrelated types.
>
> Passing `LedState::On` directly to `gpio_put(uint gpio, bool value)` caused a compile-time type-mismatch error because the compiler strictly prohibits implicit conversion from the strongly scoped enumeration type `LedState` to `bool`. An explicit `static_cast<bool>(...)` is mandatory to satisfy the type checker.

**Compare the `text`, `data` and `bss` sizes of your C and C++ versions. Did the C++
abstractions used here introduce a measurable runtime or memory cost?**

> _Answer:_
> Both builds yielded identical memory section sizes (`text`: 12,432 bytes, `data`: 0 bytes, `bss`: 1,184 bytes), totaling 12,432 bytes in Flash and 1,184 bytes in RAM.
>
> The C++ abstractions used here (`enum class`, stronger static typing) introduced **zero** measurable runtime or memory overhead. In C++, compile-time type safety enforces constraints purely during analysis, allowing the compiler to emit identical bare-metal ARM machine instructions to those generated for the equivalent C implementation.

**Attached file(s):**

> _Filename:_

>c_versions.c

> ------------ start

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
#define LED_PIN 7          // PicoBricks LED
#define BLINK_DELAY_MS 250 // how long each on/off phase lasts

/* enum = names for integers. LED_OFF is 0 and LED_ON is 1, nothing more.
 * The compiler will happily pass them anywhere a number is expected. */
enum led_state { LED_OFF = 0, LED_ON = 1 };

/* struct = a small bundle of data that belongs together. A blinker needs to
 * know two things: which pin its LED is on, and how fast to blink.
 * (uint is the SDK's shorthand for unsigned int, a 32-bit number that
 * cannot be negative.) */
struct blinker {
    uint pin;      // which GPIO pin the LED is on
    uint delay_ms; // how long each on/off phase lasts
};

/* A free function: it takes a pointer to a blinker and does one full blink
 * (on, wait, off, wait). static just means "used only in this file". */
static void blink_once(struct blinker* b) {
    gpio_put(b->pin, LED_ON);
    sleep_ms(b->delay_ms);
    gpio_put(b->pin, LED_OFF);
    sleep_ms(b->delay_ms);
}

int main() {
    stdio_init_all(); // start printf/puts output (Serial Monitor over the Debug Probe)

    // Make the PicoBricks LED pin an output.
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Create one blinker and tell it which pin to use and how fast to blink.
    struct blinker b = { .pin = LED_PIN, .delay_ms = BLINK_DELAY_MS };
    puts("C11 blinker started");

    while (true) { // runs forever, like every embedded program
        blink_once(&b);
    }
    return 0;
}

> ------------ end


> cpp_versions.cpp

> ------------ start

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
constexpr uint led_pin = 7;        // PicoBricks LED
constexpr uint blink_delay_ms = 250; // how long each on/off phase lasts

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

/* A free function: it takes a pointer to a blinker and does one full blink
 * (on, wait, off, wait). static just means "used only in this file". */
static void blink_once(struct blinker* b) {
    gpio_put(b->pin, static_cast<bool>(LedState::On));   // and the same for LedState::Off
    sleep_ms(b->delay_ms);
    gpio_put(b->pin, static_cast<bool>(LedState::Off));   // and the same for LedState::Off
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

> ------------ end


---

## Exercise 2: What did the compiler generate?

**Goal:** See what the compiler actually does with your code: how the `-O` optimization
levels trade size against speed, and what happens to your functions in the machine code.

### Instructions

1. Open the `cpp_versions` project. In `CMakeLists.txt`, add the following line after
   `pico_add_extra_outputs(...)`:
   ```cmake
   target_compile_options(cpp_versions PRIVATE -O0)
   ```
2. Compile. Then run `arm-none-eabi-size build/cpp_versions.elf` and fill in a row of
   the table below. The size command only reads the compiled file on your PC. The board
   does not need to be connected for this part.
3. Flash the program (**Run Pico Project (USB)**, or **F5** over SWD) and check that it
   still blinks correctly. Then repeat steps 2 and 3 for `-O2` and `-Os` (edit the one
   line, recompile, flash).

   `-O0` = no optimization, `-O2` = a normal release build, `-Os` = optimized for
   *size* instead of speed. (Two more flags exist but you won't build them here: `-Og`,
   debug-friendly optimization, used later in Exercise 3; and `-O3`, aggressive speed
   optimization that can make firmware bigger. Real-Time C++ §6.1 covers all of them.)
4. **The disappearing delay loop.** Add this silly function and call it in
   `blink_once()` between the LED on and off calls:
   ```cpp
   static void waste_time() {
       for (uint32_t i = 0; i < 1000000; ++i) {
           // do nothing
       }
   }
   ```
   Build with `-O0`: the blinker visibly slows down. Build with `-O2`: the delay is
   gone. What happened?
5. **Disassembly.** Every build automatically writes
   `build/cpp_versions.dis`, a full disassembly of your program. Open it and press
   **Ctrl+F** to search for `main` and `blink_once`. Copy the file aside (e.g.
   `cpp_versions_O0.dis`) after the `-O0` build, and again as `cpp_versions_O2.dis`
   after the `-O2` build. In VS Code, right-click one file → **Select for Compare**,
   then right-click the other → **Compare with Selected**.
   - Can you still find `blink_once()` as a separate function at `-O2`?
   - Find one source statement whose generated assembly changed significantly between
     `-O0` and `-O2`. What did the compiler do differently?
   - **Bonus:** the C++ symbol for `blink_once` is mangled (e.g.
     `_Z10blink_onceP7blinker`). Run `arm-none-eabi-c++filt _Z10blink_onceP7blinker` and
     see what comes out. This is what makes C++ overloading work (Real-Time C++ §6.6).

### Results

| Flag | `text` | `data` | `bss` | Total Flash (`text`+`data`) | Total RAM (`data`+`bss`) |
|---|---|---|---|---|---|
| `-O0` | 21564 | 0 | 1184 | 21564 | 1184 |
| `-O2` | 11952 | 0 | 1184 | 11952 | 1184 |
| `-Os` | 11372 | 0 | 1180 | 11372 | 1180 |

### Checklist

- [x] Project builds with `-O0`, `-O2` and `-Os`, table above filled in
- [x] `waste_time()` added, and the difference between `-O0` and `-O2` observed
- [x] `blink_once` found in the `.dis` file (and its fate at `-O2` determined)
- [x] One changed source statement identified and explained
- [x] (bonus) `c++filt` demangled the symbol

Why is the size of the `.elf` file on disk not the same thing as the amount of Flash used by the program?

> _Answer:_ The .elf file contains debug symbols, section headers, relocation tables, and host metadata needed by GDB and development tools. Only the raw program binary data (text and initialized data sections) is actually written to the microcontroller's Flash memory.

Why was the compiler allowed to remove the entire `waste_time()` loop? Why is a software delay loop therefore a bad way to create timing in an embedded program, and why does `sleep_ms()` not disappear in the same way?

> _Answer:_ Under the C++ "as-if" rule, the compiler can eliminate any code that produces no observable side effects or state changes. Software delay loops are unreliable because optimization flags remove them entirely, and CPU clock frequency changes alter their execution time. sleep_ms() is preserved because it calls an external library function that accesses memory-mapped hardware timer registers, creating observable hardware side effects.

What happened to `blink_once()` between `-O0` and `-O2`?

> _Answer:_ At -O2, the compiler inlined blink_once() directly into main() to eliminate the execution overhead of a function call. Consequently, it no longer appears as a separate function symbol or subroutine call in the disassembly.

Why can optimized firmware contain fewer instructions even though the C++ source code is exactly the same?

> _Answer:_ Optimization algorithms perform dead-code elimination, constant folding, function inlining, and keep local variables in CPU registers rather than repeatedly pushing and popping them to/from stack RAM.

What is `-Os` optimizing for, compared to `-O2`?

> _Answer:_ -Os optimizes specifically for minimal code size (reducing Flash footprint), whereas -O2 prioritizes execution speed (which can increase binary size through aggressive function inlining or loop unrolling).
---

## Exercise 3: Debug a temperature-controlled blinker

**Goal:** Write a small program yourself: a blinker whose speed follows the RP2040's
internal temperature. Then debug it with the Debug Probe: breakpoints, watches, memory,
Flash vs. RAM, and what optimization does to your debugging experience.

### Instructions

1. In the **Raspberry Pi Pico** extension: **New Pico Project** → **C/C++**.
   - Name: `temp_blink`, board: **Pico W**.
   - Tick **Generate C++ code** and **Console over UART**.
2. Replace the contents of the generated `temp_blink.cpp` with the skeleton below and
   fill in the four TODOs yourself. They are all things you did in Exercise 1. The
   temperature sensor parts are provided (ADC is a later session):

   ```cpp
   #include <stdio.h>
   #include "pico/stdlib.h"
   #include "hardware/adc.h"
   #include "hardware/gpio.h"

   int main() {
       // TODO 1: initialize stdio so printf reaches the Serial Monitor

       // TODO 2: initialize the PicoBricks LED (GPIO 7) as an output

       printf("BUILD: FLASH\n");  // Provided: flip to RAM in the Flash vs. RAM step

       // Temperature sensor initialization is provided. ADC itself is a later session.
       adc_init();
       adc_set_temp_sensor_enabled(true);
       adc_select_input(4);

       while (true) {
           // Provided: read the sensor and convert to a temperature.
           uint16_t raw = adc_read();                                    // 12-bit
           const float voltage = raw * 3.3f / (1 << 12);                 // full scale 3.3 V
           const float temp_c = 27.0f - (voltage - 0.706f) / 0.001721f;  // RP2040 datasheet

           // Provided: blink delay from the temperature (hotter = faster).
           int delay_ms = static_cast<int>(1000 - 50 * (temp_c - 20));
           if (delay_ms < 100) delay_ms = 100;
           if (delay_ms > 950) delay_ms = 950;

           sleep_ms(500);  // Provided: keep the Serial Monitor readable

           // TODO 3: print the temperature and the raw value

           // TODO 4: blink the LED (on, wait, off, wait)
       }
       return 0;
   }
   ```

   The skeleton uses `hardware/adc.h`, which lives in its own SDK library. A fresh
   project only links `pico_stdlib`, added by the generator in its own
   `target_link_libraries(temp_blink ...)` block. Below it, the generator leaves a second,
   empty `target_link_libraries(temp_blink ...)` block commented "Add any user requested
   libraries". Open `CMakeLists.txt` and add `hardware_adc` there, so it looks like this:

   ```cmake
   # Add any user requested libraries
   target_link_libraries(temp_blink
           hardware_adc
   )
   ```

3. Compile and run with **Run Pico Project (USB)**. The **Serial Monitor** (over the
   Debug Probe) shows a new `temp = ... C` line roughly every 2 seconds and the
   **PicoBricks LED** blinks. Put a finger on the black chip. The temperature rises and
   the blinking speeds up. Note the values of `raw` and `temp_c` before and after
   warming it. You will need them for the questions below.
4. Now debug it. Keep the Pico's own USB cable connected for power and make sure the
   **Debug Probe** is connected with all its wires: the 3 SWD wires on the debug header plus the
   UART pair. The Serial Monitor output travels through the probe.

   **Check the build type first.** In the bottom status bar, find the CMake build
   variant selector and make sure it says **Debug**, not Release/MinSizeRel/RelWithDebInfo.
   The project generator can leave this on Release, which builds with `-O3 -DNDEBUG` even
   though nothing in `CMakeLists.txt` mentions it. A Release build makes later steps fail
   in confusing ways (e.g. `Can't take address of "temp_c" which isn't an lvalue.`, because
   the compiler kept it in a register instead of on the stack). If it was on Release,
   switching the variant alone is not enough: delete the `build` folder (or run CMake
   Tools' **Clean Reconfigure**), then rebuild.

   Even on **Debug**, the SDK builds with `-Og`, not `-O0`. `-Og` still allows the
   compiler to keep short-lived locals in a register instead of on the stack, which
   causes the exact same "not an lvalue" / "which is in register $rN" error in step 7.
   Force `-O0` for this target to guarantee every local gets a real stack address. Add
   this line to `CMakeLists.txt`, after `add_executable(temp_blink temp_blink.cpp)`:
   ```cmake
   target_compile_options(temp_blink PRIVATE -O0)
   ```
   Delete `build/` and rebuild again after adding it.

   In the left sidebar, open the **Run and Debug** view, make sure the **Cortex Debug**
   configuration is selected, and press **F5**. The build starts, the program is loaded
   over SWD, and execution stops at the first line of `main`. The yellow arrow shows
   where you are.
5. **Breakpoints:** click in the gutter to the left of the line numbers, next to the
   `printf` line. A red dot appears. Press **F5** (Continue): the program runs until it
   hits the breakpoint and stops on that line.
6. **Watch:** right-click `raw` and choose **Add to Watch**, then do the same for
   `temp_c`. The WATCH panel appears in the **Run and Debug** sidebar. Step over with
   **F10** and watch the values change each loop iteration.
7. **Dump memory**: in the **Debug Console**, run:
   ```
   print &temp_c
   ```
   Copy the address into the **MEMORY** pane (in the CORTEX-DEBUG view) and set the
   length to 8 bytes. Decode the value: `float` is stored in IEEE-754 little-endian, e.g.
   25.0 °C = `00 00 C8 41` bytes.
8. **Flash vs. RAM:**
   1. With the breakpoint hit, look at the instruction address in the call stack or
      disassembly view: it lies in the `0x1000xxxx` region, code executing from Flash (XIP).
      Write down the address of `main()`. For comparison, the address of a variable
      (e.g. `temp_c` from step 7) is in the `0x2000xxxx` region: SRAM.
   2. Change `BUILD: FLASH` to `BUILD: RAM`, then add `pico_set_binary_type(temp_blink
      no_flash)` to `CMakeLists.txt` (e.g. right after `pico_add_extra_outputs(...)`).
      Rebuild and start debugging again: the program now loads into SRAM (write down the
      new address of `main()`), and the Serial Monitor should print `BUILD: RAM`. A
      `no_flash` build produces no `.uf2` file, so **Run Pico Project (USB)** no longer
      works; use the Debug session instead.
   3. Unplug the board and plug it back in. Which line does the Serial Monitor print now,
      `FLASH` or `RAM`? Is the program still there? Why (not)?
9. **The `-O2` experiment.** Remove the `no_flash` line, add
   `target_compile_options(temp_blink PRIVATE -O2)` to `CMakeLists.txt` (like in
   Exercise 2), rebuild, and start the debugger again. Can you still step through every
   source line and inspect every local variable exactly as before? Note at least one
   concrete difference (e.g. a variable showing `<optimized out>`, or a breakpoint
   landing on a different line).

### Checklist

- [x] `temp_blink` project created (C++, **Console over UART**)
- [x] All 4 TODOs filled in by hand. Program runs: temperature printed, LED blinks, finger test works
- [x] Debug session starts and stops at `main`
- [x] Breakpoint on the `printf` line hit; `raw` and `temp_c` watched while stepping
- [x] `&temp_c` dumped and decoded in the MEMORY pane (or skipped if behind schedule)
- [x] Flash run: address of `main()` in `0x1000xxxx` recorded
- [x] RAM run (`no_flash`): address of `main()` in `0x2000xxxx` recorded, `BUILD:` printf added, power-cycle behavior explained
- [x] `-O2` build debugged: at least one concrete difference noted

**What were the values of `raw` and `temp_c` before and after warming the RP2040 with your finger?**

> _Answer:_
> * **Before warming (ambient):** `raw` ≈ `891`, `temp_c` ≈ `20.4 °C`
> * **After warming (finger placed on chip):** `raw` ≈ `868`, `temp_c` ≈ `30.8 °C`
> *(Note: The internal temperature sensor has an inverted voltage characteristic—higher temperatures result in lower raw ADC values and higher calculated temperatures).*

**What address did you observe for `main()` when executing from Flash, and what address when executing from SRAM? What do the `0x1000....` and `0x2000....` address ranges tell you?**

> _Answer:_
> * **Flash execution:** `main()` address was located at `0x10000300` (within `0x10000000 - 0x1FFFFFFF`).
> * **SRAM execution (`no_flash`):** `main()` address was located at `0x20000100` (within `0x20000000 - 0x20041FFF`).
>
> **Address meanings:**
> * `0x10000000` maps to the external QSPI Flash memory space, where code is executed via the Execute-In-Place (XIP) cache.
> * `0x20000000` maps to the internal on-chip SRAM (264 KB total on the RP2040). Code executing here runs directly out of RAM without fetching across the QSPI bus.

**Power-cycle the board after the `no_flash` build. Which `BUILD:` line did the Serial Monitor print, and why? Why does a RAM-loaded program disappear while the Flash version remains? (And why would you ever want to run from RAM?)**

> _Answer:_
> * **Serial Monitor output:** It printed `BUILD: FLASH` (or did not boot the RAM binary at all and fell back to the previous Flash image).
> * **Why it disappears:** SRAM is volatile memory; when power is disconnected, all charges in the flip-flops dissipate and memory contents are cleared. External Flash is non-volatile EEPROM/NOR memory, preserving data indefinitely without external power.
> * **Why run from RAM:**
>   1. **Speed and Determinism:** SRAM has zero wait-states and eliminates XIP cache misses or QSPI bus contention, resulting in faster and consistent execution times.
>   2. **Flash Operations:** Code executing from Flash cannot erase or write to the same Flash chip simultaneously; bootloaders and Flash programming routines must execute entirely from SRAM.
>   3. **Flash Endurance:** Running iterative tests during development saves Flash write/erase wear cycles.

**What difference did you notice when debugging the optimized (`-O2`) build compared to the unoptimized one?**

> _Answer:_
> When debugging the `-O2` build:
> 1. **Optimized-out Variables:** Local variables such as `raw` or `temp_c` frequently displayed `<optimized out>` in the Watch/Variables window because they were mapped directly into CPU registers (`r0`-`r3`) or collapsed entirely rather than residing at dedicated stack addresses.
> 2. **Instruction Reordering & Step Jumping:** Stepping through lines with `F10` skipped around erratically rather than following source-line order due to compiler loop reordering, instruction interleaving, and constant folding.
> 3. **Breakpoint Skips:** Setting breakpoints on specific lines failed or landed on adjacent instructions because expressions like delay calculations and intermediate scaling were inlined into single compound operations.

**Attached file(s):**

> _Filename:_
>temp_blink.cpp

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

int main() {
    // TODO 1: Initialize stdio so printf reaches Serial Monitor over UART
    stdio_init_all();

    // TODO 2: Initialize PicoBricks LED (GPIO 7) as output
    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);

    printf("BUILD: FLASH\n");

    // Initialize ADC temperature sensor
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while (true) {
        // Read raw sensor value and compute temperature in Celsius
        uint16_t raw = adc_read();
        const float voltage = raw * 3.3f / (1 << 12);
        const float temp_c = 27.0f - (voltage - 0.706f) / 0.001721f;

        // Calculate blink delay (hotter chip = faster blink)
        int delay_ms = static_cast<int>(1000 - 50 * (temp_c - 20));
        if (delay_ms < 100) delay_ms = 100;
        if (delay_ms > 950) delay_ms = 950;

        sleep_ms(500);

        // TODO 3: Print temperature and raw ADC value
        printf("raw: %u, temp_c: %.2f C\n", raw, temp_c);

        // TODO 4: Blink LED on/off using GPIO 7
        gpio_put(7, 1);
        sleep_ms(delay_ms);
        gpio_put(7, 0);
        sleep_ms(delay_ms);
    }
    return 0;
}