# Exercise Session 10, Homework

Do these after Exercises 1-3, in [`instructions.md`](instructions.md). Write answers in the
`_Answer:_` blocks. Commit this file and your `.cpp` and `.h` files.

You need no PicoBricks. The Serial Monitor shows the output.

---

## Exercise 4: An error for bad data

**Goal:** Add a new error to `Error` for a value that the sensor can't measure, and
handle it with `Result`.

### Instructions

1. Create a project `checked`. Copy [`code/checked.cpp`](code/checked.cpp) into the
   project folder: it replaces the generated one. Copy `result.h` from your `errors`
   project to the same place.
2. Open the Serial Monitor, build and run. Fill in the "Before" column.
3. In `result.h`, add `OutOfRange` to `Error`. Give it the text `"out of range"` in
   `to_string()`.
4. Fill in TODO 1 and TODO 2 in `checked.cpp`.
5. Build and run. Fill in the "After" column.

| Raw value | Before (step 2) | After (step 5) |
|---|---|---|
| `0x6666` |24.9989 C |24.9989 C |
| `0x0000` | -45.0000 C| error, out of range|
| `0x6800` | 	26.0938 C|	26.0938 C |
| `0xFFFF` | 129.997 C| error, out of range|
| `0x6A00` | 27.4609 C|27.4609 C |

### Checklist

- [x] `OutOfRange` added to `Error` and to `to_string()`
- [x] TODO 1-2 filled in; table filled in

**Which lines were wrong in step 2? Could someone who only reads the output tell?**

> _Answer: The second line (-45.0000 C from 0x0000) and the fourth line (129.997 C from 0xFFFF) were outside the accepted range of −40 °C to 125 °C. Both were printed in the same format as valid readings. A reader would have to know the sensor’s range to spot the problem._
>

**`to_celsius()` could return a special number for bad data, like -1000. Why is an
`Error` better?**

> _Answer: Error::OutOfRange names the failure explicitly. The caller can check result.ok() before using the temperature and read result.error() to find out what went wrong. A special number requires every caller to remember its meaning and check for it. If a caller forgets, -1000 could end up in a graph or an average as if it were a real measurement._
>

**`Error` got a third value. Which parts of the code did you have to change? Which parts
stayed the same? Did `Result` change?**

> _Answer: We added OutOfRange to Error, added its message in to_string(), and made to_celsius() return it when the temperature was outside the range. In main(), we added the ok() check so the program prints either the temperature or the error. The raw samples and conversion formula stayed the same. Result itself stayed the same because it already accepts any value of the error type hw::Error._
>

**A real monitor gets `NoAnswer` once, and then `OutOfRange` ten times in a row. What
could it do for each?**

> _Answer: After one NoAnswer, it could log a warning, skip that reading, and retry on the next scheduled read. After ten consecutive OutOfRange errors, it could mark the sensor data as invalid and raise an alert for someone to check the sensor, its connection, or the conversion settings._
>

**Attached file(s):**

> _Filename: Exercise4_
>

*Read more (optional): Beginning C++17, Chapter 15 ("Handling Errors"), and Chapter 16
("Understanding Class Templates", "Instantiating a Class Template").*

---

## Exercise 5: Guards at compile time

**Goal:** Stop the build with `static_assert` when the ADC settings can't work.

### Instructions

1. Create a project `adc_guard`. Copy [`code/adc_guard.cpp`](code/adc_guard.cpp) into the
   project folder: it replaces the generated one. In `CMakeLists.txt`, add `hardware_adc`
   to `target_link_libraries`.
2. Open the Serial Monitor, build and run. The program reads pin 26. On a PicoBricks
   that is the pot. Without one, the numbers have no meaning: that is fine.
3. Set `ADC_BITS` to 16. Build and run. Read the output. Put 12 back.
4. Fill in TODO 1. `static_assert(condition, "message");` checks the condition during the
   build. If it is false, the build stops and shows the message.
5. Set `ADC_BITS` to 16 again. Build and read the error. Put 12 back.
6. Fill in TODO 2. `std::numeric_limits<Sample>::max()` is the biggest value a `Sample`
   can hold. `using Sample = std::uint16_t;` gives the type a second name.
7. Change `Sample` to `std::uint8_t`. Build and read the error. Then also set `ADC_BITS`
   to 8 and build again. Put `std::uint16_t` and 12 back.
8. Fill in TODO 3. In `main()`, change `AdcInput<POT_PIN>` to `AdcInput<15>`. Build and
   read the error. Put `POT_PIN` back.

| Change | Does it build? | The message |
|---|---|---|
| `ADC_BITS` is 16, no guard (step 3) |	Yes, possibly with a compiler warning | - |
| `ADC_BITS` is 16 (step 5) | 	No| ADC_BITS must be between MIN_BITS and HARDWARE_BITS|
| `Sample` is `std::uint8_t`, `ADC_BITS` is 12 (step 7) | 	No|Sample cannot hold ADC_MAX |
| `Sample` is `std::uint8_t`, `ADC_BITS` is 8 (step 7) | Yes| - |
| `AdcInput<15>` (step 8) | 	No|	ADC pin must be between GP26 and GP28 |

### Checklist

- [x] TODO 1-3 filled in; table filled in
- [x] `ADC_BITS` is 12, `Sample` is `std::uint16_t` and the pin is `POT_PIN` again

**What did the program print in step 3? When would you have found that mistake without
the guard, and when do you find it with the guard?**

> _Answer: With ADC_BITS = 16 and the guard missing, the build succeeded and your Serial Monitor repeatedly printed 0 = 0.00000 V. The compiler could warn about the invalid shift, and you could see its effect after flashing and running the program. With static_assert, the bad setting stops the build before you can flash it._
>

**What does a `static_assert` that holds cost in flash and in time? Compare it with an
`if` that checks the same thing when the program runs.**

> _Answer: Zero bytes of firmware code and zero runtime. The compiler checks it during the build. A runtime if can add instructions and take time to execute. If its condition is already known at compile time, the optimizer can remove the check, but the if still allows the build to succeed when the condition is false._
>

**Why did `std::uint8_t` build with 8 bits but not with 12? Why is comparing with
`ADC_MAX` better than checking `sizeof(Sample) >= 2`?**

> _Answer: std::uint8_t can hold values from 0 to 255. An 8-bit reading can be at most 255, so it fits. A 12-bit reading can reach 4095, so it doesn’t fit. Comparing with ADC_MAX checks whether the type can store the largest possible reading. sizeof(Sample) >= 2 would reject the valid 8-bit setup simply because uint8_t occupies one byte._
>

**Can `static_assert` check the value that `pot.read()` returns? Why? Why is the pin a
template argument here, and not a constructor argument?**

> _Answer: pot.read() gets a value from the ADC while the Pico is running, so the compiler can’t know that value during the build. The pin is a template argument so the compiler does know it when it builds AdcInput<Pin> and can reject something like AdcInput<15>. An ordinary constructor parameter can be checked with a runtime if, but it can’t be used in this static_assert._
>

**Attached file(s):**

> _Filename: Exercise5_
>

---

*Read more (optional): Beginning C++17, Chapter 2 ("Finding the Limits"), Chapter 3
("Aliases for Data Types"), Chapter 10 ("Static Assertions"), and Chapter 16 ("Nontype
Class Template Parameters", "Using static_assert() in a Class Template"). Real-Time C++, Sect. 3.9 ("static
assert"), p. 47, Sect. 3.10 ("Using `<limits>`"), pp. 47-48, and Sect. A.4
("Compile-Time Assert"), p. 367.*
