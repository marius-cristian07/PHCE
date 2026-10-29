# Troubleshooting Guide, Exercise Session 8

## Serial Monitor shows nothing

1. Check the Debug Probe wires. You need 3 SWD wires. You also need UART on GP0/GP1.
2. Tick **Console over UART** in the new project. Or check `CMakeLists.txt` for
   `pico_enable_stdio_uart(<name> 1)`.
3. `stdio_init_all();` goes first in `main()`.

## The first lines are missing

The Pico prints them before the Serial Monitor is open. Open the Serial Monitor first,
then run. Or press reset on the Pico.

## `undefined reference to 'hw::...'`

A `.cpp` file is missing from `add_executable(...)`. Every `.cpp` in the folder goes
there. The `.h` files don't.

## `undefined reference to 'adc_init'` or `'i2c_init'`

Add `hardware_adc` (Exercise 1) or `hardware_i2c` (Exercises 2 and 3) to
`target_link_libraries`.

## `multiple definition of 'main'`

`climate.cpp` is in the `climate_display` project. Remove it: `climate_display.cpp` has
its own `main()`.

## `cannot declare variable ... to be of abstract type`

The class still has a `= 0` function without a body. Check that each derived class has `read()` **and** `unit()`, spelled the same,
with `const`.

## `marked 'override', but does not override`

The function does not match the one in the base class. Check the name, the parameters
and `const`. On purpose in step 8 of Exercise 1 and step 6 of Exercise 4.

## `'const class hw::Sensor' has no member named ...`

The function is only in a derived class. A `Sensor&` only knows what `Sensor` has.

## `sensors`: the light value goes up when you cover it

TODO 1 is upside down. Less light gives a higher raw value.

## `climate`: the scan finds nothing

Check `SDA_PIN` is 4, `SCL_PIN` is 5 and the bus is `i2c0`. Check the board sits well on
the PicoBricks.

## `climate`: the scan has no 0x70

The scan must come after `hw::Shtc3 sensor {bus};`. The constructor wakes the SHTC3. Asleep,
it does not answer.

## `climate`: `measure failed` every time

1. TODO 1: return `false` only when `bus_.read()` fails, not when it works.
2. `ADDRESS` must be `0x70` (step 8 sets it wrong on purpose).

## `climate`: always 0 C and 0 %

TODO 2 is missing: `temperature_` and `humidity_` are never set.

## `climate_display`: the OLED stays dark

1. TODO 1 is missing: `show()` returns `false` and sends nothing.
2. Check the Serial Monitor says `show() worked`.

## `climate_display`: only "Climate" shows

TODO 2 is missing, or `measure()` failed: then the screen says `measure failed`. See the
`climate` entries above.

## `devices`: both tables look the same

Step 7 needs both: `virtual` removed in `Device`, and `override` removed in `Buzzer` and
`Button`.

## `thermostat`: `uninitialized reference member`

A reference member must be set in the member initializer list, after the `:`. It can't be
set in the constructor body.
