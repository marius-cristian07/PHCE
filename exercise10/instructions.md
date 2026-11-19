# Exercise Session 10

Work through the exercises in order. Write answers in the `_Answer:_` blocks. Commit this
file and your `.cpp` and `.h` files.

Exercises 1-3 are for the session. Exercises 4 and 5 are homework.
See [`homework.md`](homework.md).

---

## Exercise 1: Logging with levels

**Goal:** Log a climate monitor at four levels, once with C macros and once with a C++
function template, and switch levels off at compile time.

### Instructions

1. Create a project `logging`. Copy the files inside [`code/sensor/`](code/sensor/) and
   [`code/logging/`](code/logging/) into the project folder, next to `CMakeLists.txt`.
   Copy the files, not the two folders: the project has no subfolders. `logging.cpp`
   replaces the generated one.
2. In `CMakeLists.txt`, add `i2c_bus.cpp` and `shtc3.cpp` to
   `add_executable(logging ...)`. Add `hardware_i2c` to `target_link_libraries`.
3. Read `c_log.h`. In a macro, `...` takes any number of arguments and `__VA_ARGS__` puts
   them back. Fill in TODO 1.
4. Read `log.h`. A call looks like `logging::log<Severity::Info>("%.1f C", temperature)`:
   the level goes between `<` and `>`.
5. Fill in TODO 2 with `if constexpr (condition)`. It is an `if` that the compiler
   decides. If the condition is false, the code inside is left out of the program. Compare
   two levels with `>=`.
6. Fill in TODO 3 in `logging.cpp`.
7. Open the Serial Monitor, build and run. Breathe on the sensor until a warning shows,
   then an error. The sensor is the part marked "TEMP & HUM" on the PicoBricks.
8. Set the logging level to warning. In `c_log.h`, set `C_LOG_LEVEL` to
   `C_LOG_LEVEL_WARNING`. In `log.h`, set `ACTIVE_LEVEL` to `Severity::Warning`. Build
   and run. Breathe on the sensor again.
9. Keep both levels at warning. Below the two "measuring" lines, add
   `C_LOG_DEBUG("%d", smaple);`. The typo is on purpose. Build. Then change the line to
   `logging::log<Severity::Debug>("%d", smaple);`. Build and read the error. Remove the
   line.
### Checklist

- [ ] TODO 1-3 filled in
- [ ] A warning and an error seen, both ways
- [ ] With both levels at warning, no `DEBUG` or `INFO` line shows
- [ ] The typo tried both ways and removed

**Which lines were gone in step 8? For each way: what takes a message that is switched
off out of the program?**

> _Answer:_
>

**In `logging::log<Severity::Info>(...)` the level is a template argument. Why can't it
be a normal argument, like `logging::log(Severity::Info, ...)`?**

> _Answer:_
>

**Step 9: which line built with the typo, and which did not? Why? Which one do you want
in a real project?**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 10 ("Defining Preprocessor Macros",
"Defining Function-Like Macros", "Logical Preprocessing Directives"), Chapter 9
("Nontype Template Parameters"), and Chapter 8 ("String Views: The New
Reference-to-const-string"). Real-Time C++, Sect. 3.8 ("Generalized Constant
Expressions with constexpr"), pp. 46-47, and Sect. 5.9 ("Variadic Templates"),
pp. 102-104.*

---

## Exercise 2: Exceptions or `Result`

**Goal:** Handle a sensor that does not answer in two ways, with a `Result` return value
and with an exception, and measure what each one costs.

### Instructions

1. Create a project `errors`. Copy the files inside [`code/sensor/`](code/sensor/) and
   [`code/errors/`](code/errors/) into the project folder, next to `CMakeLists.txt`.
   `errors.cpp` replaces the generated one.
2. In `CMakeLists.txt`, add `i2c_bus.cpp` and `shtc3.cpp` to
   `add_executable(errors ...)`. Add `hardware_i2c` to `target_link_libraries`.
3. Read `result.h`. A function that returns `hw::Result<float, hw::Error>` can end with
   `return 21.5f;` or with `return hw::Error::NoAnswer;`.
4. Read `main()` in `errors.cpp`. There are two sensors. `inside` is the SHTC3 on the
   PicoBricks. `outside` sits on a bus with nothing connected, so it never answers.
5. Fill in TODO 1. Use `bus.is_free()` and `sensor.measure()`.
6. Open the Serial Monitor, build and run. Then, in a terminal inside VS Code, run
   `arm-none-eabi-size build/errors.elf`, like in Session 2. Fill in the first column.
7. Switch exceptions on. The Pico SDK builds without them. In `CMakeLists.txt`, add this
   line above `pico_sdk_init()`:

   ```cmake
   set(PICO_CXX_ENABLE_EXCEPTIONS 1)
   ```

   The code between `#ifdef __cpp_exceptions` and `#endif` in `errors.cpp` is now part
   of the program.
8. Read the class `SensorError` in `errors.cpp`. An exception is an object: this one
   carries the `Error`. Fill in TODO 2. It is thrown like this:
   `throw SensorError {hw::Error::NoAnswer};`.
9. Read `print_or_catch()` below it. `try` holds the code that can throw. The code in
   `catch (const SensorError& error)` runs only if a `SensorError` was thrown.
10. Build and run. Run `arm-none-eabi-size build/errors.elf` again. Fill in the second
    column.

| | Exceptions off (step 6) | Exceptions on (step 10) |
|---|---|---|
| `text` (bytes) | | |
| 1000 failed reads with `Result` (us) | | |
| 1000 failed reads with exceptions (us) | - | |

### Checklist

- [ ] TODO 1-2 filled in
- [ ] `outside` prints `error, no answer` and `caught, no answer`
- [ ] Table filled in

**How many bytes did the exceptions add to `text`? How much longer does one failed read
take with an exception than with `Result`?**

> _Answer:_
>

**Look at the first line of `read_temperature()` and of `read_temperature_or_throw()`.
Which one shows that it can fail? What must the caller do in each case?**

> _Answer:_
>

**Give one reason to pick exceptions, and one to pick `Result`, for a program on the
Pico.**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 15 ("Handling Errors", "Understanding
Exceptions", "Throwing an Exception", "The Exception-Handling Process", "Class Objects as
Exceptions", "Deriving Your Own Exception Classes"). Real-Time C++, Sect. 10.7 ("Recognizing and Handling Memory Limitations"),
pp. 215-217.*

---

## Exercise 3: A watchdog

**Goal:** Let the watchdog timer reset the Pico when the program hangs, and follow it in
the log.

### Instructions

1. Create a project `watchdog`. Copy the files inside [`code/sensor/`](code/sensor/) and
   [`code/watchdog/`](code/watchdog/) into the project folder, next to `CMakeLists.txt`.
   `watchdog.cpp` replaces the generated one. Copy your `log.h` from the `logging`
   project to the same place. Set `ACTIVE_LEVEL` in it back to `Severity::Debug`.
2. In `CMakeLists.txt`, add `i2c_bus.cpp` and `shtc3.cpp` to
   `add_executable(watchdog ...)`. Add `hardware_i2c` and `hardware_watchdog` to
   `target_link_libraries`.
3. Read `main()` in `watchdog.cpp`. Find the bug: a loop that never ends while the
   button is down.
4. Open the Serial Monitor, build and run. Press the button. Wait ten seconds. Then
   press Run again to get the program back.
5. Fill in TODO 1. `watchdog_caused_reboot()` returns `true` if the watchdog caused the
   last reset.
6. Fill in TODO 2: `watchdog_enable(WATCHDOG_MS, PAUSE_ON_DEBUG)`. It starts a timer that
   resets the Pico when it runs out.
7. Fill in TODO 3: `watchdog_update()`. It starts the timer again from `WATCHDOG_MS`.
8. Build and run. Press the button. Count the seconds from `stuck` to the next line.
9. Set `UPDATE_MS` to 5000. Build and run. Don't touch the button. Watch for a minute.
   Then put 1000 back.

| | Without the watchdog (step 4) | With the watchdog (step 8) |
|---|---|---|
| The next line after `stuck` | | |
| Seconds from `stuck` to that line | - | |

### Checklist

- [ ] TODO 1-3 filled in
- [ ] After `stuck`, the Pico starts again by itself and logs a warning
- [ ] Table filled in; `UPDATE_MS` is 1000 again

**What did the Pico do after `stuck` in step 4, and in step 8? How long did step 8 take?
Where does that time come from?**

> _Answer:_
>

**After a reset, how does the program know that the watchdog did it? Why is that worth a
warning?**

> _Answer:_
>

**What happened in step 9? Nothing was stuck. What must be true for `UPDATE_MS` and
`WATCHDOG_MS`?**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Real-Time C++, Sect. 8.1 ("The Startup Code"), pp. 155-157, and
Sect. 8.7 ("Jump to main() and Never return"), pp. 167-168.*

Exercises 4 and 5 continue in [`homework.md`](homework.md).
