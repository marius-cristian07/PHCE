# Exercise Session 7, Homework

Do these after Exercises 1-3, in [`instructions.md`](instructions.md). Write answers in the
`_Answer:_` blocks. Commit this file and your `.cpp` file.

Both exercises use one starter, [`code/fraction_unary.cpp`](code/fraction_unary.cpp). It
has the `Fraction` class from Exercise 3, without `+=`, `+` and `*`. It has a non-member
`operator==`, as an example. You need no hardware. The Serial Monitor shows the output.

---

## Exercise 4: Unary operators

**Goal:** Add `-`, `++` and `--` to `Fraction`, in the prefix and postfix forms.

In `main()`, `f` is a `Fraction`: `Fraction f {3, 4};`.

A unary member operator has no parameter: `Fraction operator-() const`.

- Prefix `++f`: `Fraction& operator++()`. Returns `*this`.
- Postfix `f++`: `const Fraction operator++(int)`. The `int` is a dummy, never used. It
  copies `*this`, calls `++(*this)` and returns the copy.

Adding 1 to `a/b` gives `(a + b)/b`.

### Instructions

1. Create a project `fraction_unary`. Copy in
   [`code/fraction_unary.cpp`](code/fraction_unary.cpp).
2. Fill in TODO 1. `operator-` is `const`: `-f` gives a new `Fraction` and leaves `f` as
   it is.
3. Read the prefix `++`. Fill in TODO 2: the prefix `--`, the same way.
4. Read the postfix `++`. Fill in TODO 3: the postfix `--`, the same way.
5. Fill in TODO 4. Open the Serial Monitor, build and run. Fill in the table.

| Line | Your output |
|---|---|
| `-f`, then `f` |-3/4, then 3/4 |
| `++f`, then `f` | 7/4, then 7/4|
| `f++`, then `f` | 7/4, then 11/4|
| `--f`, then `f` | 7/4, then 7/4|
| `f--`, then `f` | 7/4, then 3/4|

### Checklist

- [x] TODO 1-4 filled in; the table filled in
- [x] `-f` leaves `f` as it is
- [x] The postfix forms use the prefix forms

**Why does `f++` show the old value? Why must postfix return a copy, not a reference?**

> _Answer: f++ shows the old value because it saves a copy of the fraction before increasing it. It must return a copy because the original fraction changes, and returning a reference would give us the updated value instead of the old one._
>

**Why is `operator-` `const`, but `operator++` is not?**

> _Answer: operator- is const because it creates a new negative fraction without changing the original. operator++ is not const because it modifies the numerator of the existing fraction by adding the denominator._
>

**Add `show("f   = ", Fraction {1, 0});` and run. What do you get? Why does `reduce()` not
stop it? How could the constructor protect the class?**

> _Answer: The program may crash or stop working because the denominator is zero, causing division by zero in reduce(). The reduce() function doesn't prevent this because it doesn't check whether the denominator is zero before dividing. The constructor could protect the class by checking if the denominator is zero and rejecting invalid fractions, for example by throwing std::invalid_argument._
>

**Attached file(s):**

> _Filename: fraction_unary.cpp_
>

*Read more (optional): Beginning C++17, Chapter 12 ("Overloading Unary Operators",
"Overloading the Increment and Decrement Operators").*

---

## Exercise 5: Print with `<<`

**Goal:** Print a `Fraction` with `std::cout << f`.

The left operand is `std::cout`, so `operator<<` must be a non-member:

```cpp
std::ostream& operator<<(std::ostream& stream, const Fraction& fraction);
```

It uses the public `numerator()` and `denominator()`, like the non-member `operator==` in
the starter.

### Instructions

1. Keep working in `fraction_unary.cpp`.
2. Fill in TODO 5. Write it like `operator==` above it. Write into `stream`, not into
   `std::cout`. Return `stream`.
3. Fill in TODO 6. Build and run.
4. In `show()`, replace `fraction.print()` with `<<`. Then delete `print()` from the class.
   Build and run. The output must not change.

### Checklist

- [x] TODO 5 and 6 filled in
- [x] `show()` uses `<<`; `print()` is gone
- [x] Same output as before

**Change `operator<<` to return `void` and build. What is the error for TODO 6? Why?
Then change it back.**

> _Answer: The compiler reports an error because we cannot chain << operators anymore. When operator<< returns void, it doesn't return the stream needed for the next << operation. Returning std::ostream& allows multiple outputs to be chained together in one statement._
>

**Why does `operator<<` write into `stream`, not into `std::cout`?**

> _Answer: It writes into stream so the operator can work with different output streams, not just std::cout. For example, we could print a fraction to a file or a string stream. This makes the operator more flexible and reusable._
>

**Attached file(s):**

> _Filename: fraction_unary.cpp_
>

---

*Read more (optional): Beginning C++17, Chapter 12 ("Overloading the << Operator for
Output Streams", "Member vs. Nonmember Functions").*
