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
| `0x6666` | | |
| `0x0000` | | |
| `0x6800` | | |
| `0xFFFF` | | |
| `0x6A00` | | |

### Checklist

- [ ] `OutOfRange` added to `Error` and to `to_string()`
- [ ] TODO 1-2 filled in; table filled in

**Which lines were wrong in step 2? Could someone who only reads the output tell?**

> _Answer:_
>

**`to_celsius()` could return a special number for bad data, like -1000. Why is an
`Error` better?**

> _Answer:_
>

**`Error` got a third value. Which parts of the code did you have to change? Which parts
stayed the same? Did `Result` change?**

> _Answer:_
>

**A real monitor gets `NoAnswer` once, and then `OutOfRange` ten times in a row. What
could it do for each?**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
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
| `ADC_BITS` is 16, no guard (step 3) | | - |
| `ADC_BITS` is 16 (step 5) | | |
| `Sample` is `std::uint8_t`, `ADC_BITS` is 12 (step 7) | | |
| `Sample` is `std::uint8_t`, `ADC_BITS` is 8 (step 7) | | - |
| `AdcInput<15>` (step 8) | | |

### Checklist

- [ ] TODO 1-3 filled in; table filled in
- [ ] `ADC_BITS` is 12, `Sample` is `std::uint16_t` and the pin is `POT_PIN` again

**What did the program print in step 3? When would you have found that mistake without
the guard, and when do you find it with the guard?**

> _Answer:_
>

**What does a `static_assert` that holds cost in flash and in time? Compare it with an
`if` that checks the same thing when the program runs.**

> _Answer:_
>

**Why did `std::uint8_t` build with 8 bits but not with 12? Why is comparing with
`ADC_MAX` better than checking `sizeof(Sample) >= 2`?**

> _Answer:_
>

**Can `static_assert` check the value that `pot.read()` returns? Why? Why is the pin a
template argument here, and not a constructor argument?**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

---

*Read more (optional): Beginning C++17, Chapter 2 ("Finding the Limits"), Chapter 3
("Aliases for Data Types"), Chapter 10 ("Static Assertions"), and Chapter 16 ("Nontype
Class Template Parameters", "Using static_assert() in a Class Template"). Real-Time C++, Sect. 3.9 ("static
assert"), p. 47, Sect. 3.10 ("Using `<limits>`"), pp. 47-48, and Sect. A.4
("Compile-Time Assert"), p. 367.*
