# Exercise Session 9

Work through the exercises in order. Write answers in the `_Answer:_` blocks. Commit this
file and your `.cpp` and `.h` files.

Exercises 1-3 are for the session. Exercises 4 and 5 are homework.
See [`homework.md`](homework.md).

---

## Exercise 1: A progress bar from widgets

**Goal:** Show how far the pot is turned as a `ProgressBar`, built from a `Label` and two
`Rectangle`s.

### Instructions

1. Create a project `gui`. Copy the files inside [`code/oled/`](code/oled/) and
   [`code/gui/`](code/gui/) into the project folder, next to `CMakeLists.txt`. Copy the
   files, not the two folders: the project has no subfolders. `gui.cpp` replaces the
   generated one.
2. In `CMakeLists.txt`, add `i2c_bus.cpp`, `ssd1306.cpp`, `shape.cpp`, `text.cpp` and
   `progress_bar.cpp` to `add_executable(gui ...)`. Add `hardware_i2c` and `hardware_adc`
   to `target_link_libraries`.
3. Read `widget.h`, `shape.h`, `text.h` and `progress_bar.h`. Find what each class adds.
4. Fill in TODO 1 in `shape.cpp`: `display.draw_rect(x, y, width, height, filled)`. Get
   the values with `x()`, `y()`, `width()` and `height()`.
5. Fill in TODO 2 in `progress_bar.cpp`, in `draw()`. Each part has its own `draw()`.
6. Fill in TODO 3 in `gui.cpp`.
7. Build and run. Turn the knob.

### Checklist

- [ ] TODO 1-3 filled in
- [ ] The bar and the number follow the knob, from 0 % to 100 %

**Write down each class in the `gui` namespace and its base class. Which of them can't
be made as an object? Why?**

> _Answer:_
>

**`ProgressBar` and `Widget`, `ProgressBar` and `Label`: which pair is inheritance, which
is composition? Why does `ProgressBar` not derive from `Rectangle`?**

> _Answer:_
>

**`Rectangle::draw()` uses `x()`, not `x_`. Why?**

> _Answer:_
>

**The loop in `main()` only knows `const gui::Widget*`. How many `draw()` functions run
for one frame? Name them.**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 13 ("Inheritance vs. Aggregation",
"Constructors in a Derived Class"), and Chapter 14 ("Virtual Functions", "Pure Virtual
Functions", "Abstract Classes"). Real-Time C++, Sect. 4.4 ("Dynamic Polymorphism"),
pp. 69-70, and Sect. 4.7 ("Class Relationships"), pp. 72-74.*

---

## Exercise 2: `virtual` or a template

**Goal:** Draw the widgets with a function template, and measure it against the
`virtual` method.

### Instructions

1. Create a project `dispatch`. Copy [`code/dispatch/dispatch.cpp`](code/dispatch/dispatch.cpp)
   into the project folder, next to `CMakeLists.txt`: it replaces the generated one. From
   your `gui` project folder, copy all `.h` and `.cpp` files to the same place, except
   `gui.cpp`.
2. In `CMakeLists.txt`, add the same five `.cpp` files to `add_executable(dispatch ...)`.
   Add `hardware_i2c` to `target_link_libraries`.
3. In `screen.h`, add this to `Screen`, below `draw()`:

   ```cpp
   template <typename... Widgets>
   void draw_all(const Widgets&... widgets)
   {
       (widgets.draw(display_), ...);
   }
   ```

   `typename... Widgets` stands for any number of types. The line in the body makes one
   `draw()` call for each argument, in order: `draw_all(a, b)` becomes
   `a.draw(display_), b.draw(display_)`.
4. In `dispatch.cpp`, read the loop that fills `dynamic_us`. `time_us_32()` gives the
   microseconds since the Pico started. Dispatch means picking which `draw()` runs.
   Dynamic: decided at run time, with `virtual`. Static: decided at compile time.
5. Fill in TODO 1: one call of `screen.draw_all()` with the three widgets.
6. Open the Serial Monitor, build and run. Fill in the tables.
7. Find the class `Dot` in `dispatch.cpp`. It has no base class. Fill in TODO 2. Build and
   run. Find the dot on the screen, right of the countdown text.
8. Add `&dot` to `widgets` and make `WIDGET_COUNT` 4. Build and read the **first** error.
   Take `&dot` out and put 3 back.

| | Dynamic: loop over `widgets` | Static: `draw_all()` |
|---|---|---|
| Time for 1000 frames (us) | | |
| Time for one frame (us) | | |

| | `Rectangle` | `Label` | `ProgressBar` |
|---|---|---|---|
| `sizeof` (bytes) | | | |

### Checklist

- [ ] `draw_all()` added; TODO 1-2 filled in
- [ ] Both tables filled in
- [ ] The dot shows on the screen during the countdown
- [ ] Error read (step 8)

**How big is the difference for one frame? Is `virtual` expensive in this program? Where
does the time of a frame go?**

> _Answer:_
>

**What was the error in step 8? Why can `draw_all()` draw a `Dot`, but `widgets` can't
hold one?**

> _Answer:_
>

**`draw_all()` is called with `(title, bar, frame)` and with `(dot)`. How many functions
does the compiler make from the template? What are their parameters?**

> _Answer:_
>

**What can the loop over `widgets` do that `draw_all()` can't?**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 9 ("Function Templates", "Creating
Instances of a Function Template"). Real-Time C++, Sect. 4.5 ("The Real Overhead of
Dynamic Polymorphism"), pp. 70-71, Sect. 5.3 ("Template Member Functions"), pp. 90-93,
Sect. 5.7 ("Static Polymorphism"), pp. 97-99, and Sect. 5.9 ("Variadic Templates"),
pp. 102-104.*

---

## Exercise 3: Widgets without `virtual`

**Goal:** Build the same widgets with class templates and no `virtual`, using the
Curiously Recurring Template Pattern (CRTP).

### Instructions

1. Create a project `crtp`. Copy the files inside [`code/oled/`](code/oled/) and
   [`code/crtp/`](code/crtp/) into the project folder, next to `CMakeLists.txt`. Copy the
   files, not the two folders. `crtp.cpp` replaces the generated one.
2. In `CMakeLists.txt`, add `i2c_bus.cpp`, `ssd1306.cpp`, `widgets.cpp` and
   `progress_bar.cpp` to `add_executable(crtp ...)`. Add `hardware_i2c` to
   `target_link_libraries`.
3. Read `widget.h` and `widgets.h`. Find `class Rectangle : public Shape<Rectangle>`.
   Follow `Rectangle` up: `Shape<Rectangle>` hands it on to `Widget<Rectangle>`. Search
   both files for `virtual`.
4. Fill in TODO 1 in `widget.h`, in `draw()`. `this` points to a `const Widget<Derived>`.
   Turn it into a `const Derived*` with `static_cast`. Call `draw_widget(display)` on it.
5. Fill in TODO 2 in `progress_bar.cpp`.
6. Open the Serial Monitor, build and run. Fill in the table. Take the Exercise 2 column
   from your Exercise 2 answers.

| | Exercise 2 | Exercise 3 |
|---|---|---|
| `draw_all()`, time for 1000 frames (us) | | |
| `sizeof(gui::Rectangle)` (bytes) | | |
| `sizeof(gui::Label)` (bytes) | | |
| `sizeof(gui::ProgressBar)` (bytes) | | |

### Checklist

- [ ] TODO 1-2 filled in
- [ ] The screen counts down from 100 s and the bar shrinks
- [ ] Table filled in

**How does `Widget<Derived>::draw()` reach `Rectangle::draw_widget()` without `virtual`?
When is the function picked: when the program is built, or when it runs?**

> _Answer:_
>

**How many bytes did each class lose? What was in those bytes? Why did `ProgressBar` lose
the most?**

> _Answer:_
>

**What is the base class of `Label`? Of `Rectangle`? Can one `std::array` of base class
pointers hold `title`, `bar` and `frame`, like `widgets` in Exercise 1? Why?**

> _Answer:_
>

**Was CRTP faster? Give one reason to pick `virtual`, and one to pick CRTP.**

> _Answer:_
>

**Attached file(s):**

> _Filename:_
>

*Read more (optional): Beginning C++17, Chapter 16 ("Understanding Class Templates",
"Defining Class Templates", "Instantiating a Class Template"). Real-Time C++, Sect. 5.4
("Template Class Types"), pp. 93-94, Sect. 5.7 ("Static Polymorphism"), pp. 97-99, and
Sect. 2.6 ("Reaching the Right Efficiency"), pp. 30-33.*

Exercises 4 and 5 continue in [`homework.md`](homework.md).
