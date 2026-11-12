# Troubleshooting Guide, Exercise Session 9

## Serial Monitor shows nothing

1. Check the Debug Probe wires. You need 3 SWD wires. You also need UART on GP0/GP1.
2. Tick **Console over UART** in the new project. Or check `CMakeLists.txt` for
   `pico_enable_stdio_uart(<name> 1)`.
3. `stdio_init_all();` goes first in `main()`.

## The first lines are missing

The Pico prints them before the Serial Monitor is open. Open the Serial Monitor first,
then run. Or press reset on the Pico.

## `i2c_bus.h: No such file or directory`, or another `.h` file

The files are in a subfolder like `oled/` or `gui/`. Move them out, next to
`CMakeLists.txt`. The project has no subfolders.

## `undefined reference to 'hw::...'` or `'gui::...'`

A `.cpp` file is missing from `add_executable(...)`. Every `.cpp` in the folder goes
there. The `.h` files don't.

## `undefined reference to 'i2c_init'` or `'adc_init'`

Add `hardware_i2c` (Exercises 1-3) and `hardware_adc` (Exercise 1) to
`target_link_libraries`.

## `multiple definition of 'main'`

`gui.cpp` is in the `dispatch` project. Remove it: `dispatch.cpp` has its own `main()`.

## `I2C bus is stuck` prints and the OLED stays dark

A device on the PicoBricks holds an I2C line low, and the clock pulses that `I2cBus`
sends at the start did not free it. Unplug the USB cables of the Pico and the PicoBricks,
wait a few seconds and plug them in again. A reset or a new flash does not help.

## The OLED stays dark, with no message

Check `SDA_PIN` is 4 and `SCL_PIN` is 5. Check the Pico sits well on the PicoBricks.

## `gui`: only "Potentiometer" shows

1. TODO 3: `bar` must be in `widgets`, and `WIDGET_COUNT` must be 2.
2. TODO 2 is missing: `ProgressBar::draw()` draws nothing.

## `gui`: the text follows the knob, but there is no border and no fill

TODO 1 is missing: `Rectangle::draw()` draws nothing.

## `'x_' is private within this context`

`x_` and `y_` are private in `Widget`. Use `x()` and `y()`. The same goes for `width()`,
`height()` and `text()`.

## `'class gui::Screen' has no member named 'draw_all'`

Step 3 of Exercise 2 is missing, or it went into another `screen.h` than the one in this
project.

## `parameter packs not expanded with '...'`

The line in `draw_all()` needs its outer brackets: `(widgets.draw(display_), ...);`

## `dispatch`: `static:` is only a few microseconds

TODO 1 is missing: the timed loop draws nothing.

## `cannot convert 'const Dot*' to 'const gui::Widget*'`

On purpose, in step 8 of Exercise 2. Take `&dot` out of `widgets` and put `WIDGET_COUNT`
back to 3.

## `crtp`: the screen stays empty, but the times print

TODO 1 is missing: `Widget<Derived>::draw()` does nothing. If only the bar is missing,
it is TODO 2.

## `'const class gui::...' has no member named 'draw_widget'`

`draw()` looks for `draw_widget()` in the class named in the error. Check the name, and
that it ends in `const`.

## `average`: `no matching function for call to 'average(int, double, int)'`

On purpose, in step 5. All three values must have the same type. Or name the type:
`average<double>(1, 2.5, 3)`.

## `average`: `'T' was not declared in this scope`

`template <typename T>` is missing on the line above the function.

## `devices_static`: `'const class Button' has no member named 'run_self_test'`

`Button` needs `run_self_test()`, spelled the same, with `const`. `self_test()` is only in
`Device<Derived>`.

## `devices_static`: the button's `self_test()` says `beep`

`Button` derives from `Device<Buzzer>`. Make it `Device<Button>`, in the class and in the
constructor.
