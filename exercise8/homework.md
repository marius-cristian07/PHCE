# Exercise Session 8, Homework

Do these after Exercises 1-3, in [`instructions.md`](instructions.md). Write answers in the
`_Answer:_` blocks. Commit this file and your `.cpp` files.

You need no hardware. The Serial Monitor shows the output.

---

## Exercise 4: Inheritance vs. polymorphism

**Goal:** See what inheritance gives alone, and what `virtual` adds.

The starter is [`code/devices.cpp`](code/devices.cpp). `Buzzer` derives from `Device`:

- `name()`: only in `Device`. `Buzzer` inherits it.
- `describe()`: in both, **not** `virtual`.
- `self_test()`: in both. `virtual` in `Device`, `override` in `Buzzer`.

`check(const Device& device)` calls `describe()` and `self_test()` on any `Device`.

### Instructions

1. Create a project `devices`. Copy in [`code/devices.cpp`](code/devices.cpp).
2. Fill in TODO 1: a class `Button`, like `Buzzer`.
3. Fill in TODO 2: make a `Button` and call its functions.
4. Fill in TODO 3: check `button` like `buzzer`.
5. Open the Serial Monitor, build and run. Fill in the first table.
6. In `Device`, remove `virtual` from `self_test()`. Build and read the errors.
7. Now also remove both `override`s. Build and run. Fill in the second table.
8. Put `virtual` and `override` back.

With `virtual` (step 5):

| Line | On the object | Through `check()` |
|---|---|---|
| `buzzer` `describe()` | | |
| `buzzer` `self_test()` | | |
| `button` `describe()` | | |
| `button` `self_test()` | | |

Without `virtual` (step 7):

| Line | On the object | Through `check()` |
|---|---|---|
| `buzzer` `describe()` | | |
| `buzzer` `self_test()` | | |
| `button` `describe()` | | |
| `button` `self_test()` | | |

### Checklist

- [ ] TODO 1-3 filled in; both tables filled in
- [ ] Errors read (step 6)
- [ ] `virtual` and `override` back in

**Through `check()`, why does `buzzer` give a different `describe()` than on the object?
Which type picks the function: the type of the reference, or of the object?**

> _Answer:_
>

**What were the errors in step 6? What do they protect you from?**

> _Answer:_
>

**What is the difference between inheritance and polymorphism in this example? Use
`name()` and `self_test()` to explain.**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 14 ("Calling Inherited Functions",
"Virtual Functions", "Using override", "Using References to Call Virtual Functions").
Real-Time C++, Sect. 4.4 ("Dynamic Polymorphism"), pp. 69-70, and Sect. 4.5 ("The Real
Overhead of Dynamic Polymorphism"), pp. 70-71.*

---

## Exercise 5: Inheritance vs. composition and aggregation

**Goal:** Build a thermostat that has a heater and uses a sensor, instead of being a
sensor.

The starter is [`code/thermostat.cpp`](code/thermostat.cpp). `FakeSensor` and `Heater`
print a line when they are made and when they are gone.

`InheritingThermostat` is the IT variant. It **is** a `FakeSensor` (inheritance) and
**has** a `Heater` member. You write the CT variant, `ComposingThermostat`:

- It has a `Heater heater_` member. It owns the heater: **composition**.
- It gets a `FakeSensor` in its constructor and keeps `const FakeSensor& sensor_`. It
  uses the sensor, it does not own it: **aggregation**. Like `bus_` in `Shtc3`.

### Instructions

1. Create a project `thermostat`. Copy in [`code/thermostat.cpp`](code/thermostat.cpp).
2. Read `InheritingThermostat` and the IT part of `main()`.
3. Fill in TODO 1. Initialize `sensor_` in the member initializer list, like `bus_` in
   `Shtc3`.
4. Fill in TODO 2: make `ct`, make it cold and print if `ct` is heating, like for `it`.
5. Fill in TODO 3: print the sensor after `ct` is gone.
6. Open the Serial Monitor, build and run.
7. Follow the `made` and `gone` lines. Mark in the output when `it`, `ct`, their heaters and
   the sensors are made and gone.

### Checklist

- [ ] TODO 1-3 filled in
- [ ] Both variants print `heating: yes`
- [ ] Lifetimes marked (step 7)

**In the IT variant, `main()` calls `it.set(COLD_CELSIUS)`. Why does that compile? Why is it odd
for a thermostat?**

> _Answer:_
>

**When `ct` is gone, which `gone` lines print? Why is the sensor still there?**

> _Answer:_
>

**In Exercise 2, someone writes `class Shtc3 : public I2cBus`. It would work. Why is it still wrong? What
could code that holds an `Shtc3` do then?**

> _Answer:_
>

**You want two thermostats on one sensor. Can the IT variant do it? Can the CT variant? How?**

> _Answer:_
>

**Fill in for the CT variant: `ComposingThermostat` and `Heater` are composition /
aggregation. `ComposingThermostat` and `FakeSensor` are composition / aggregation. Why?**

> _Answer:_
>

**What is the difference between inheritance, aggregation and composition? Give one
example of each from this session.**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

---

*Read more (optional): Beginning C++17, Chapter 13 ("Inheritance vs. Aggregation",
"Destructors Under Inheritance", "The Order in Which Destructors Are Called"). Real-Time
C++, Sect. 4.7 ("Class Relationships"), pp. 72-74.*
