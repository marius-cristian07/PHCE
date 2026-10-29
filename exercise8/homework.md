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
| `buzzer` `describe()` |makes a sound |a device |
| `buzzer` `self_test()` | 	beep|	beep |
| `button` `describe()` | reads a press|a device |
| `button` `self_test()` | press me|press me |

Without `virtual` (step 7):

| Line | On the object | Through `check()` |
|---|---|---|
| `buzzer` `describe()` |makes a sound |a device |
| `buzzer` `self_test()` | beep|no test |
| `button` `describe()` | reads a press|a device |
| `button` `self_test()` | press me|no test |

### Checklist

- [x] TODO 1-3 filled in; both tables filled in
- [x] Errors read (step 6)
- [x] `virtual` and `override` back in

**Through `check()`, why does `buzzer` give a different `describe()` than on the object?
Which type picks the function: the type of the reference, or of the object?**

> _Answer: Because describe() is not virtual, C++ selects the function based on the type of the reference, not the actual object. Inside check(), the reference is const Device&, so it calls Device::describe() instead of Buzzer::describe(). When called directly on the buzzer object, it uses Buzzer::describe()._
>

**What were the errors in step 6? What do they protect you from?**

> _Answer: The compiler reported that Buzzer::self_test() and Button::self_test() were marked override but did not override any virtual function. This happened because we removed virtual from Device::self_test(). These errors protect us from accidentally defining functions that don't correctly override the base class functions._
>

**What is the difference between inheritance and polymorphism in this example? Use
`name()` and `self_test()` to explain.**

> _Answer: Inheritance allows Buzzer and Button to reuse name() from the Device class without implementing it themselves. Polymorphism allows check() to call the correct self_test() implementation depending on the actual object, even though it only receives a Device& reference. Inheritance reuses functionality, while polymorphism allows different objects to behave differently through the same interface._
>

**Attached file(s):**

> _Filename: Exercise4_
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

- [x] TODO 1-3 filled in
- [x] Both variants print `heating: yes`
- [x] Lifetimes marked (step 7)

**In the IT variant, `main()` calls `it.set(COLD_CELSIUS)`. Why does that compile? Why is it odd
for a thermostat?**

> _Answer: It compiles because InheritingThermostat inherits from FakeSensor, including its set() function. This is odd because a thermostat is supposed to control heating using a sensor, not be a sensor itself. It should read temperature from a separate sensor instead of setting its own temperature._
>

**When `ct` is gone, which `gone` lines print? Why is the sensor still there?**

> _Answer: When ct is destroyed, only Heater gone prints because the thermostat owns the heater. The FakeSensor remains alive because it was created separately in main() and ct only holds a reference to it. FakeSensor gone prints later, when the sensor goes out of scope._
>

**In Exercise 2, someone writes `class Shtc3 : public I2cBus`. It would work. Why is it still wrong? What
could code that holds an `Shtc3` do then?**

> _Answer: It is wrong because an SHTC3 is a sensor, not an I2C bus. Inheritance would incorrectly make it an I2C bus and expose functions such as read(), write(), and probe(). Code holding an Shtc3 could then use it to communicate directly with other I2C devices, which is not its responsibility. It should use an existing bus through aggregation._
>

**You want two thermostats on one sensor. Can the IT variant do it? Can the CT variant? How?**

> _Answer: The IT variant cannot naturally share one sensor because each InheritingThermostat contains its own inherited FakeSensor object. The CT variant can share one sensor because both thermostats can store references to the same FakeSensor object._
>

**Fill in for the CT variant: `ComposingThermostat` and `Heater` are composition /
aggregation. `ComposingThermostat` and `FakeSensor` are composition / aggregation. Why?**

> _Answer: ComposingThermostat and Heater have a composition relationship because the thermostat owns the heater, and the heater is destroyed when the thermostat is destroyed.

ComposingThermostat and FakeSensor have an aggregation relationship because the thermostat only stores a reference to an existing sensor. The sensor is not owned by the thermostat and can exist independently._
>

**What is the difference between inheritance, aggregation and composition? Give one
example of each from this session.**

> _Answer: Inheritance represents an is-a relationship, where a class inherits functionality from another class. Example: Buzzer inherits from Device in Exercise 4.

Composition represents an owning has-a relationship, where one object contains another and controls its lifetime. Example: ComposingThermostat owns a Heater in Exercise 5.

Aggregation represents a non-owning uses-a relationship, where an object uses another independently existing object. Example: Shtc3 holds a reference to I2cBus in Exercise 2._
>

**Attached file(s):**

> _Filename: Exercise5_
>

---

*Read more (optional): Beginning C++17, Chapter 13 ("Inheritance vs. Aggregation",
"Destructors Under Inheritance", "The Order in Which Destructors Are Called"). Real-Time
C++, Sect. 4.7 ("Class Relationships"), pp. 72-74.*
