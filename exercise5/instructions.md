# Exercise Session 5

Work through the exercises in order. Write answers in the `_Answer:_` blocks. Commit this
file and your `.cpp` files.

Exercises 1-3 are for the session. Exercises 4 and 5 are homework.
See [`homework.md`](homework.md).

Create projects like in Session 4. Use **New Pico Project** → **C/C++**. Pick board
**Pico W**. Tick **Generate C++ code** and **Console over UART**.

---

## Exercise 1: Reading the potentiometer

**Goal:** Read the potentiometer with the ADC. Turn the raw number into volts and ohms.

So far every pin was digital. Digital pins are only 0 or 1. The ADC measures the voltages
in between.

The Pico's ADC has 12 bits. That gives 4096 steps, 0 to 4095. 4095 means 3.3 V.

The potentiometer ("pot") is a knob on a 10 kΩ resistor. One end of the resistor is at
3.3 V, the other at GND. A contact, the wiper, slides along it as you turn the knob and
connects to GP26. Halfway along, it picks up half of 3.3 V. The ohms from the wiper to GND
change the same way.

### Instructions

1. Create a project `pot_adc`. Copy in [`code/pot_adc.cpp`](code/pot_adc.cpp). In
   `CMakeLists.txt`, add `hardware_adc` to `target_link_libraries`.
2. Fill in TODO 1 in `raw_to_volts()`. It gets `raw` and returns volts.
3. Fill in TODO 2 in `raw_to_ohms()`. Set `POT_OHMS` to the value on your pot. `B10K`
   means 10 kΩ.
4. Open the Serial Monitor, build and run. Turn the knob fully both ways. Fill in the
   table.
5. Now leave the knob still. Watch `raw` for a few seconds. Note how much it jumps.
6. Add `<< "  9-bit " << (raw >> 3)` to the print. `>> 3` drops the 3 lowest bits. A
   9-bit ADC would see this.

| Knob | `raw` | volts | ohms |
|---|---|---|---|
| fully left |0 | 0.0| 0|
| middle | 2047|1.65 | 5000|
| fully right | 4095|3.3 | 10000|

`raw` jumps by about ___ steps with the knob still. The 9-bit value jumps by ___.

### Checklist

- [x] TODO 1 and 2 filled in
- [x] Table filled in
- [x] Jumps noted, 12-bit and 9-bit

**How many millivolts is one 12-bit step?**

> _Answer:_
>One 12-bit step is approximately 0.806 mV. Step size = 3300/4095 = 0.8059mV  

**The knob was still. Why did `raw` move?**

> _Answer:raw moved due to analog noise, electrical interference on the breadboard, slight voltage ripple on the 3.3V power rail, and inherent ADC sampling jitter on the RP2040. Because each step is extremely small (~0.81 mV), even sub-millivolt noise causes the lowest bits of the reading to fluctuate._
>

**Did the 9-bit value jump less? Why?**

> _Answer:Yes. Bit-shifting right by 3 bits (raw >> 3) divides the value by $2^3 = 8$, which effectively discards the 3 least significant (lowest) bits. Because the noise jitter was small enough to fit within those lower 3 bits, removing them filters out the small fluctuations and produces a stable reading._
>

**Attached file(s):**

> _Filename:_ pot_adc.cpp
>

*Read more (optional): Programming Embedded Systems, Sect. 13.1.4.1 ("Analog circuits"):
analog vs. digital, a knob on a variable resistor, and why analog signals are noisy.
Real-Time C++, Sect. 6.13 ("Consider Advantageous Hardware Dimensioning"): a voltage
divider in front of an ADC. Its `raw2mv()` function does the same job as `raw_to_volts()`.*

---

## Exercise 2: Disco in Darkness

**Goal:** Light the LED when it gets dark.

The light sensor works like the pot. It puts a voltage on GP27. Less light gives a higher
number.

### Instructions

1. Create a project `light_led`. Copy in [`code/light_led.cpp`](code/light_led.cpp). In
   `CMakeLists.txt`:
   - Add `hardware_adc` and `pico_status_led` to `target_link_libraries`.
     `pico_status_led` drives the RGB LED on GP6, used from step 6.
   - Below `add_executable(...)`, add this line. It tells the SDK which pin the RGB LED
     is on: `target_compile_definitions(light_led PRIVATE PICO_DEFAULT_WS2812_PIN=6)`
2. Fill in TODO 1. Dark turns the LED on.
3. Build and run. Note `raw` in normal light. Cover the sensor, and note it again.
   A dim room can read higher than a covered sensor in bright light.
4. Set `DARK_THRESHOLD` halfway between your two numbers. Rebuild and test.
5. Half cover the sensor, so `raw` sits near `DARK_THRESHOLD`. Watch the LED.
6. **Disco:** fill in TODO 2. The RGB LED now glows blue in the dark, together with the
   red LED.
7. **Disco:** fill in TODO 3. Keep the position in `COLORS` in a variable declared before
   the loop. `% COLORS.size()` brings it back to 0 after the last colour.

| Normal light | Covered | Your threshold |
|---|---|---|
|77 |720 | 398.5|

### Checklist

- [x] TODO 1 filled in; the LED turns on in the dark
- [x] Threshold set from your own numbers
- [x] LED checked with `raw` near the threshold
- [x] TODO 2 filled in; the RGB LED glows blue when dark
- [x] TODO 3 filled in; the colours change when dark

**Why two thresholds? What would `is_dark = raw > DARK_THRESHOLD;` do in step 5?**

> _Answer:_ Two thresholds create hysteresis (a deadband). In step 5, when raw sits right near DARK_THRESHOLD, natural ADC noise causes raw to rapidly jump back and forth across a single threshold value. With is_dark = raw > DARK_THRESHOLD;, the LED would rapidly flicker on and off. Using two thresholds prevents this flickering.
>

**Between the two thresholds, what does the LED do?**

> _Answer:_ The LED maintains its previous state (it stays ON if it was already ON, and stays OFF if it was already OFF). Because neither the if nor the else if condition triggers when raw falls in the gap between the two thresholds, is_dark keeps its current value.
>

**The ADC has several inputs. Which line picks the sensor's?**

> _Answer:_ adc_select_input(LIGHT_ADC_INPUT); (which passes 1 to select ADC channel 1 connected to GP27).
>

**Attached file(s):**

> _Filename:_ light_led.cpp
>

---

## Exercise 3: Macros vs. `constexpr`

**Goal:** See what the preprocessor does. Then use it only where it helps.

The preprocessor runs before the compiler. It only swaps text. `#define LED_PIN 7` swaps
every `LED_PIN` for `7`.

Beginning C++17 says: no `#define` constants. They have no type and no scope.
Use `constexpr` for constants instead.

`#if` is different. It picks which lines get compiled. The other lines are deleted first.

### Instructions

1. Create a project `macros`. Copy in [`code/macros.cpp`](code/macros.cpp). Add
   `hardware_adc` and `pico_cyw43_arch_none` to `target_link_libraries`.
2. Read the three switches at the top. They pick which code gets compiled.
3. Build and run. Turn the pot past halfway. The Pico W's own LED lights up.
4. Fill in TODO 1. Write the constants like earlier sessions: `constexpr uint LED_PIN {7};`
   Rebuild. Nothing should change.
5. Look at what `DOUBLE(3 + 4)` printed. It is printed once, above the `[DEBUG]` lines, so
   scroll up. Is it 14?
6. Fill in TODO 2. Define the following constexpr function:
   `constexpr int double_value(int x)`. Delete `DOUBLE` and print `double_value(3 + 4)`.
7. Set `USE_BUILTIN_LED` to `0`. Fill in both TODO 3s. Now the red LED on GP7 reacts.
8. In `led_set()`, turn `#if USE_BUILTIN_LED`, `#else` and `#endif` into a normal
   `if (USE_BUILTIN_LED)` and `else`, with the same two lines inside. Build with
   `USE_BUILTIN_LED` still `0`. Read the error, then undo.
9. Set `EXERCISE` to `2`. Set the `THRESHOLD` to the value from Exercise 2. Cover the
   sensor: the LED lights.
10. Set `EXERCISE` to `3`. Build and read the **first** error. The errors after it follow
    from it.
11. Delete the `#define EXERCISE` line. Build and read the first error again. Then put it
    back.
12. Put `//` in front of `#define DEBUG_MODE`. Rebuild. The `[DEBUG]` lines are gone.

### Checklist

- [x] TODO 1 and 2 filled in; `double_value(3 + 4)` prints 14
- [x] TODO 3 filled in; the GP7 LED works
- [x] Plain `if` error read (step 8)
- [x] `EXERCISE` set to 2, 3 and deleted
- [x] `DEBUG_MODE` turned off

**What did `DOUBLE(3 + 4)` print? Write the text the preprocessor made.**

> _Answer:_ It printed 11. The preprocessor made the text 3 + 4 * 2. Because multiplication takes operator precedence over addition, it evaluated as $3 + (4 \times 2) = 11$.
>

**Why does `double_value(3 + 4)` give 14?**

> _Answer:_ double_value is a proper C++ function. C++ evaluates the argument expression (3 + 4) first—producing 7—and then passes 7 into the function, calculating $7 \times 2 = 14$.
>

**Say you had kept `#define LED_PIN 7`. What would `int LED_PIN {5};` inside a function
turn into? Why is that a problem?**

> _Answer:_ It would turn into int 7 {5};. This is a compilation error because 7 is a literal integer value, not a valid variable identifier name.
>

**What was the error in step 8? Why does `#if` avoid it?**

> _Answer:_ The error was a missing symbol/driver compiler error for cyw43_arch_gpio_put. A standard C++ if statement requires both branches to be compiled into the program regardless of runtime conditions. #if avoids it because preprocessor directives strip out the unselected code blocks entirely before the compiler ever sees them.
>

**You deleted `#define EXERCISE`. Which error came first? Why?**

> _Answer:_ The #error "EXERCISE must be 1 or 2" preprocessor error came first. In preprocessor conditional directives (#if), an undefined macro evaluates to 0. Since 0 is neither 1 nor 2, the preprocessor hit the #else branch and triggered #error. Because the preprocessor runs before the compiler, this halted compilation before C++ scope checks occurred.
>

**`DEBUG_MODE` is off. Where did the printing code go?**

> _Answer:_ It was completely removed by the preprocessor before compilation. When DEBUG_MODE is not defined, DEBUG_LOG(...) expands to empty text, so no printing instructions or string literals exist in the compiled binary.
>

**Attached file(s):**

> _Filename:_ macros.cpp
>

*Read more (optional): Beginning C++17, Chapter 10 ("Defining Preprocessor Macros",
"Logical Preprocessing Directives"). Real-Time C++, Sect. 1.11 ("Compile-Time Constant")
and 3.8 ("Generalized Constant Expressions with constexpr").*

Exercises 4 and 5 continue in [`homework.md`](homework.md).
