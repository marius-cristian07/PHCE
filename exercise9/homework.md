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
| `average(2, 4, 7)` |4.33333 |4 |
| `average(2.5, 4.5, 6.5)` |4.5	 |4.5	 |

### Checklist

- [x] TODO 1-3 filled in; table filled in
- [x] Error read (step 5)
- [x] The return type is `double` again

**What is `T` in `average(2, 4, 7)`? In `average(2.5, 4.5, 6.5)`? Who picks it? How many
functions does the compiler make from the template in step 3?**

> _Answer: In average(2, 4, 7), T is int. In average(2.5, 4.5, 6.5), T is double. The compiler automatically determines T from the argument types through template argument deduction. It creates two function specializations, one for int and one for double._
>

**Why does `average()` return `double` and not `T`? What happened in step 4?**

> _Answer: It returns double to preserve decimal values, even when the inputs are integers. In step 4, changing the return type to T caused integer results to be truncated. For example, average(2, 4, 7) returned 4 instead of 4.33333, because T was int._
>

**What was the error in step 5? Why does `average<double>(1, 2.5, 3)` build?**

> _Answer: The compiler couldn't deduce a single type T because the arguments contained both int and double. Using average double explicitly sets T to double, allowing the integers to be implicitly converted to doubles, so the function compiles._
> 

**Which types work as `T`? What must a type be able to do?**

> _Answer: Numeric types such as int, float, and double work as T. A type must support addition (+) between its values, and the sum must support division by COUNT (a double). The resulting value must also be convertible to double, since that is the function's return type._
>

**Attached file(s):**

> _Filename: Exercise4_
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
| `buzzer` `describe()` |makes a sound |a device |
| `buzzer` `self_test()` | beep|beep |
| `button` `describe()` |reads a press |a device |
| `button` `self_test()` |press me |press me |

| | Session 8, `virtual` | Session 9, class template |
|---|---|---|
| `sizeof(Buzzer)` (bytes) |28 bytes |24 bytes |

### Checklist

- [x] TODO 1-3 filled in; both tables filled in
- [x] Session 8 size measured (step 6)

**Compare the first table with your "With `virtual`" table from Session 8. What is the
same? What does the work of `virtual` here?**

> _Answer: Both versions produce the same results: describe() calls the base-class version through check(), while self_test() calls the derived class's implementation. In Session 8, virtual selects the correct self_test() at runtime. In Exercise 5, CRTP achieves the same behavior using static_cast at compile time._
>

**Session 8 had one `check(const Device&)`. How many `check()` functions does the
compiler make here? What are their parameters?**

> _Answer: The compiler creates two specializations of check():
- check(const Device Buzzer&)
- check(const Device Button&)
Each specialization is generated for a different template argument._
>

**Session 8 could keep a `Buzzer` and a `Button` in one array of `const Device*`. Can
this version? Why?**

> _Answer: No, because Device Buzzer and Device Button are different template specializations with no common non-template base class. Therefore, they cannot both be stored in a single array of const Device* pointers like in Session 8._
>

**Which `Buzzer` is smaller? By how much? What is gone?**

> _Answer: The CRTP version is smaller: 24 bytes compared to 28 bytes in the virtual version, saving 4 bytes (14.3%). The hidden virtual function table pointer (vptr) is no longer needed because CRTP uses compile-time dispatch._
>

**Give one thing the `virtual` version does better, and one thing the class template
version does better.**

> _Answer: The virtual version provides better runtime flexibility, allowing different derived objects to be stored and accessed through common base-class pointers.
The class template (CRTP) version provides better memory efficiency by eliminating virtual function pointers and allowing compile-time dispatch._
>

**Attached file(s):**

> _Filename: Exercise 5_
>

---

*Read more (optional): Beginning C++17, Chapter 16 ("Understanding Class Templates",
"Defining Class Templates", "Instantiating a Class Template"). Real-Time C++, Sect. 4.5
("The Real Overhead of Dynamic Polymorphism"), pp. 70-71, and Sect. 5.7 ("Static
Polymorphism"), pp. 97-99.*
