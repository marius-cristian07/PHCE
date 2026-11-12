# Exercise Session 9, Homework

Do these after Exercises 1-3, in [`instructions.md`](instructions.md). Write answers in the
`_Answer:_` blocks. Commit this file and your `.cpp` files.

You need no hardware. The Serial Monitor shows the output.

---

## Exercise 4: An average for any type

**Goal:** Write one function template that gives the average of three values of any
number type.

The starter is [`code/average.cpp`](code/average.cpp).

### Instructions

1. Create a project `average`. Copy in [`code/average.cpp`](code/average.cpp).
2. Fill in TODO 1. A function template starts with `template <typename T>`. Then `T` is
   used like a type.
3. Fill in TODO 2. Open the Serial Monitor, build and run. Fill in the first column.
4. Change the return type of `average()` from `double` to `T`. Build and run. Fill in the
   second column. Put `double` back.
5. Fill in TODO 3 with `average(1, 2.5, 3)`. Build and read the error. Then name the type
   yourself: `average<double>(1, 2.5, 3)`. Build and run.

| Call | Returns `double` | Returns `T` |
|---|---|---|
| `average(2, 4, 7)` | | |
| `average(2.5, 4.5, 6.5)` | | |

### Checklist

- [ ] TODO 1-3 filled in; table filled in
- [ ] Error read (step 5)
- [ ] The return type is `double` again

**What is `T` in `average(2, 4, 7)`? In `average(2.5, 4.5, 6.5)`? Who picks it? How many
functions does the compiler make from the template in step 3?**

> _Answer:_
>

**Why does `average()` return `double` and not `T`? What happened in step 4?**

> _Answer:_
>

**What was the error in step 5? Why does `average<double>(1, 2.5, 3)` build?**

> _Answer:_
>

**Which types work as `T`? What must a type be able to do?**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 9 ("Function Templates", "Creating
Instances of a Function Template", "Template Type Parameters", "Explicit Template
Arguments"). Real-Time C++, Sect. 5.1 ("Template Functions"), pp. 85-87.*

---

## Exercise 5: Devices without `virtual`

**Goal:** Build `Device`, `Buzzer` and `Button` from Session 8 again with a class
template, and compare with the `virtual` version.

The starter is [`code/devices_static.cpp`](code/devices_static.cpp). `Device` is now a
class template, `Device<Derived>`. `Buzzer` derives from `Device<Buzzer>`:

- `name()`: only in `Device<Derived>`. `Buzzer` inherits it.
- `describe()`: in both, like in Session 8.
- `self_test()`: only in `Device<Derived>`. It calls `run_self_test()` of `Derived`.
- `run_self_test()`: only in `Buzzer`.

### Instructions

1. Create a project `devices_static`. Copy in
   [`code/devices_static.cpp`](code/devices_static.cpp).
2. Open your `devices.cpp` from Session 8 next to it. Find what happened to `virtual` and
   `override`.
3. Fill in TODO 1: a class `Button`, like `Buzzer`.
4. Read the function template `check()`. Fill in TODO 2 and 3.
5. Open the Serial Monitor, build and run. Fill in the table.
6. Copy the line that prints `sizeof(Buzzer)` into `main()` of your Session 8
   `devices.cpp`. Build and run that project. Write down both sizes.

| Line | On the object | Through `check()` |
|---|---|---|
| `buzzer` `describe()` | | |
| `buzzer` `self_test()` | | |
| `button` `describe()` | | |
| `button` `self_test()` | | |

| | Session 8, `virtual` | Session 9, class template |
|---|---|---|
| `sizeof(Buzzer)` (bytes) | | |

### Checklist

- [ ] TODO 1-3 filled in; both tables filled in
- [ ] Session 8 size measured (step 6)

**Compare the first table with your "With `virtual`" table from Session 8. What is the
same? What does the work of `virtual` here?**

> _Answer:_
>

**Session 8 had one `check(const Device&)`. How many `check()` functions does the
compiler make here? What are their parameters?**

> _Answer:_
>

**Session 8 could keep a `Buzzer` and a `Button` in one array of `const Device*`. Can
this version? Why?**

> _Answer:_
>

**Which `Buzzer` is smaller? By how much? What is gone?**

> _Answer:_
>

**Give one thing the `virtual` version does better, and one thing the class template
version does better.**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

---

*Read more (optional): Beginning C++17, Chapter 16 ("Understanding Class Templates",
"Defining Class Templates", "Instantiating a Class Template"). Real-Time C++, Sect. 4.5
("The Real Overhead of Dynamic Polymorphism"), pp. 70-71, and Sect. 5.7 ("Static
Polymorphism"), pp. 97-99.*
