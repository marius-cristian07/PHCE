# Exercise Session 6

Work through the exercises in order. Write answers in the `_Answer:_` blocks. Commit this
file and your `.cpp` and `.h` files.

Exercises 1-3 are for the session. Exercises 4 and 5 are homework.
See [`homework.md`](homework.md).
The homework draws diagrams in [draw.io](https://app.diagrams.net).

Create projects like in Session 5. Use **New Pico Project** → **C/C++**. Pick board
**Pico W**. Tick **Generate C++ code** and **Console over UART**.

All three exercises build the same program: the pot sets how bright the red LED glows.
Exercise 1 writes it with plain functions. Exercises 2 and 3 rewrite it with classes.

---

## Exercise 1: Dimming the LED with PWM

**Goal:** Set the LED brightness with the potentiometer, using PWM.

In Session 5 the ADC turned a voltage into a number. Now we go the other way: a number
into a brightness.

PWM (pulse-width modulation) and the duty cycle are from the lecture.
The Pico has PWM hardware. It has a counter. The counter counts 0, 1, 2, ... up to a
number called `wrap`, then starts at 0 again. One full round is one period. The pin is
high while the counter is below `level`, and low for the rest of the round.

Example with `wrap` 9 and `level` 3: the counter counts 0 to 9, which is 10 counts per
period. The pin is high for 0, 1 and 2, and low for 3 to 9. That is 3 of 10 counts, so a
30% duty cycle.

```
counter: 0 1 2 3 4 5 6 7 8 9 0 1 2 3 ...
pin:     ‾ ‾ ‾ _ _ _ _ _ _ _ ‾ ‾ ‾ _ ...
```

So the duty cycle is `level / (wrap + 1)`. The counter steps once per tick of the system
clock, so it goes round many thousands of times a second. A question below works out how
often. The hardware does all this by itself. The CPU only sets `level`.

The PWM hardware has 8 slices. Each slice drives two pins. `pwm_gpio_to_slice_num()` gives
the slice that drives a pin.

The ADC gives 12 bits (0 to 4095). We use `wrap` 255, so `level` goes from 0 to 255:
8 bits. In Session 5 you used `raw >> 3` to get 9 bits. Here `>> 4` drops 4 bits: 12 - 4 = 8.

At `level` 255 the pin is high for 255 of 256 counts. It is still low for one. To keep it
high all the time, `level` must be above `wrap`. The starter does this for you: at the top
level it sets `FULL_ON_LEVEL`, which is `wrap + 1`. So the knob goes from off to fully on.
That is why `level` prints 256, not 255, with the knob fully right.

### Instructions

1. Create a project `pwm_led`. Copy in [`code/pwm_led.cpp`](code/pwm_led.cpp). In
   `CMakeLists.txt`, add `hardware_adc` and `hardware_pwm` to `target_link_libraries`.
2. Fill in TODO 1. `gpio_set_function()` hands a pin to a piece of hardware.
3. Fill in TODO 2 with `pwm_set_wrap()`.
4. Fill in TODO 3. Shift `raw` right with `>>`, by `ADC_TO_PWM_SHIFT` bits, like
   `raw >> 3` in Session 5. Open the Serial Monitor, build and run. Turn the knob fully
   both ways. Fill in the table.
5. Set `PWM_WRAP` to `4095` and `ADC_TO_PWM_SHIFT` to `0`. Rebuild. Turn the knob slowly
   near the dark end. Compare with 8 bits.

| Knob | `raw` | `level` | duty cycle |
|---|---|---|---|
| fully left |12 |0 | 0|
| middle |2048 |128 | 50|
| fully right |4095 |256 |100 |

### Checklist

- [x] TODO 1, 2 and 3 filled in; the knob dims the LED
- [x] Table filled in
- [x] 12-bit version tried (step 5)

**Why shift by 4? What would shifting left do?**

> _Answer: _We shift right by 4 because the ADC gives a 12-bit value (0–4095), but PWM uses 8 bits (0–255). Shifting right by 4 divides the value by 16, making it fit. Shifting left would multiply the value by 16, making it too large for the PWM range.
>

**The system clock runs at 125 MHz. One period is `wrap + 1` counts. How many periods
per second with `wrap` 255? Could you see it blink?**

> _Answer: _One period has 256 counts, so: 125,000,000 / 256=488,281.25 HZ. That is approximately 488,281 periods per second. We cannot see the LED blinking because the frequency is much too high for the human eye.
>

**What changed with 12 bits in step 5? Why?**

> _Answer: With 12 bits, the LED brightness can be controlled more precisely because we have 4096 levels instead of 256. The brightness changes are smaller and smoother, especially at low brightness. This happens because we use the full ADC value without shifting it. The PWM frequency also becomes 16 times lower. _
>

**Attached file(s):**

> _Filename: pwm_led.cpp_
>

*Read more (optional): Real-Time C++, Sect. 9.4 ("A Software PWM Template Class"),
pp. 181-185: the counter, the duty cycle and PWM hardware. Sect. 4.1 ("Object Oriented
Programming"), pp. 61-66: an LED dimmed by a PWM signal.*

---

## Exercise 2: One class does everything

**Goal:** Put the dimmer into one class. Then see why one class is too much.

You used structs in Session 4 (`Toggler`). They held only data. You also saw a class in
Session 3: `LedController`. Now we write our own.

A member initializer list sets the member variables before the body runs:
`Dimmer(std::uint32_t led_pin, std::uint32_t pot_pin) : led_pin_ {led_pin}, ...`. You saw one in Session 3:
`: pin_(pin)`. We use `{}` now, like for other variables. Beginning C++17 says: prefer it
to assigning in the body. Keep the order of the class.

A local object is destroyed, and its destructor runs, at the `}` of its block.

A `const` member function promises not to change the object. Only `const` member
functions can be called on a `const` object, or through a reference-to-`const`
(Session 4: `const Note&`).

Member variables end in `_` here, like `pin_` in Session 3. Then they don't clash with
parameter names.

### Instructions

1. Create a project `dimmer_class`. Copy in [`code/dimmer_class.cpp`](code/dimmer_class.cpp).
   Add `hardware_adc` and `hardware_pwm` like in Exercise 1.
2. Fill in TODO 1. The syntax is in the intro above: `: led_pin_ {led_pin}, ...`.
   Read the constructor body: it is the setup from Exercise 1, with the member variables
   instead of the global constants.
3. Fill in TODO 2. It is like `update()`, but it only returns `level_`.
4. Fill in TODO 3. Open the Serial Monitor, build and run. For 10 seconds the knob dims the
   LED. Then watch the last lines and the LED.
5. Remove `const` from `level()`. Build and read the error. Then put `const` back.
6. List every job `Dimmer` does. Then imagine a second LED on another pin, set by the
   light sensor on GP27. Could you reuse `Dimmer` for it?

### Checklist

- [x] TODO 1-3 filled in; the knob dims the LED
- [x] Destructor seen: LED off, `Dimmer destroyed` printed
- [x] `const` error read (step 5)
- [x] Jobs of `Dimmer` listed (step 6)

**In what order did `Dimmer created`, `Dimmer destroyed` and `main() is done` print?
Why there?**

> _Answer: First Dimmer created, then Dimmer destroyed, and finally main() is done. This happens because the constructor runs when the object is created, the destructor runs when the object goes out of scope after 10 seconds, and then the program continues executing main()._
>

**What was the error in step 5? Why does `print_level()` need `level()` to be `const`?**

> _Answer: The compiler gave an error because print_level() receives a const Dimmer&, but level() was not marked as const. A const object can only call const member functions, so we need to add const to level() to promise that it won't modify the object._
>

**Which jobs does `Dimmer` do? What goes wrong with a second `Dimmer` on GP27?**

> _Answer: The Dimmer class initializes the ADC and PWM, reads the sensor, calculates the brightness, controls the LED, and prints the values. With a second Dimmer on GP27, both objects would share the same ADC hardware. Each constructor selects its ADC input, so the second object would change the input used by the first one. As a result, both objects could read the same sensor instead of their own._
>

**Attached file(s):**

> _Filename: dimmer_class.cpp_
>

*Read more (optional): Beginning C++17, Chapter 11 ("Classes and Object-Oriented
Programming", "Defining a Class", "Constructors", "Using a Member Initializer List",
"const Objects and const Member Functions", "Destructors"). Real-Time C++, Sect. 4.2
("Objects and Encapsulation"), pp. 66-67, and Sect. 4.9 ("Constant Methods"), pp. 75-79.*

---

## Exercise 3: Many classes, many files

**Goal:** Split the dimmer into three classes in their own files, in namespaces.

Each class gets one job:

- `PwmLed`: an LED on a PWM pin. It knows only PWM.
- `Potentiometer`: a pot on an ADC pin. It knows only the ADC.
- `Application`: uses a pot and an LED. It knows only the rule "pot sets brightness".

Each class has a header file (`.h`) and a source file (`.cpp`). The header holds the class
definition. Any file that uses the class includes it. The source file holds the member
function bodies. There, each name gets the class in front: `hw::PwmLed::set_level()`.
`#ifndef` at the top of a header stops it being included twice.

Namespaces and `static` members are from the lecture. `PwmLed` and `Potentiometer` live
in `namespace hw`, `Application` in `namespace app`. A namespace without a name keeps its
contents inside one `.cpp` file. `PwmLed::MAX_LEVEL` is a `static` member.
One folder per namespace is usual; here all files share one, to keep it simple.

`Application` holds references to the pot and the LED. It uses them but does not own them.
`main()` creates them and hands them over.

### Instructions

1. Create a project `dimmer_multi`. Copy all files from
   [`code/dimmer_multi/`](code/dimmer_multi/) into it. `dimmer_multi.cpp` replaces the
   generated one.
2. In `CMakeLists.txt`, add the three new `.cpp` files to `add_executable(...)`:
   `add_executable(dimmer_multi dimmer_multi.cpp pwm_led.cpp potentiometer.cpp application.cpp)`
   Add `hardware_adc` and `hardware_pwm` like before.
3. Read the three headers. Find each class's public functions and its namespace.
4. Read `pwm_led.cpp`. Compare it with `Dimmer` from Exercise 2. Then fill in TODO 1 in
   `potentiometer.cpp`. In the constructor of `Dimmer`, `adc_select_input()` picks the
   input.
5. Fill in TODO 2 in `application.cpp`. Open the Serial Monitor, build and run. It should
   work like Exercise 1.
6. In `dimmer_multi.cpp`, remove the `hw::` from `hw::PwmLed led`. Build and read the
   **first** error. Then put `hw::` back.
7. Go back to step 6 of Exercise 2. Which classes could you reuse now for the light-sensor
   LED?

### Checklist

- [x] All files added; the project builds
- [x] TODO 1 and 2 filled in; the knob dims the LED
- [x] Namespace error read (step 6)
- [x] Reuse question answered (step 7)

**Why does `Potentiometer::read()` select its input every time? What would break without
it, with two pots?**

> _Answer: The Pico has one ADC shared between multiple input pins. Each time we read a potentiometer, we need to select its input. Without this, two potentiometers could read from the same ADC channel, giving incorrect values._
>

**What was the first error in step 6? What did the compiler suggest?**

> _Answer: The compiler gave an error saying PwmLed was not declared in this scope. It suggested using hw::PwmLed because the class belongs to the hw namespace._
>

**`ADC_TO_PWM_SHIFT` sits in a namespace without a name in `application.cpp`. Who can use
it?**

> _Answer: Only code inside application.cpp can use ADC_TO_PWM_SHIFT. The anonymous namespace keeps it private to that source file, so other files cannot access it directly._
>

**`MAX_LEVEL` is `static`. How can you write `hw::PwmLed::MAX_LEVEL` without any `PwmLed`
object? Where could `Application` use it?**

> _Answer: Because MAX_LEVEL is static, it belongs to the class itself rather than an individual object. We can access it using hw::PwmLed::MAX_LEVEL without creating an object. Application could use it when calculating or limiting the LED brightness._
>

**For the light-sensor LED, which classes can you reuse as they are? What would you
change?**

> _Answer: We can reuse PwmLed and Potentiometer without changing their code. We would create another LED object with a different PWM pin and another sensor object using GP27. We could also reuse Application if we want the light sensor to control brightness in the same way. If we want different behavior, we would change the brightness calculation in Application._
>

**Attached file(s):**

> _Filename: dimmer_multi folder_
>

*Read more (optional): Beginning C++17, Chapter 10 ("Namespaces"). Chapter 11
("Defining Functions and Constructors Outside the Class", "Static Members of a Class").
Real-Time C++, Sect. 3.4 ("Organization with Namespaces"), pp. 39-41, and Sect. 4.7
("Class Relationships"), pp. 72-74.*

Exercises 4 and 5 continue in [`homework.md`](homework.md).
