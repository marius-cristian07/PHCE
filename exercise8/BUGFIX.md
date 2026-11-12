# Bugfix, Exercise Session 8: the program hangs on the I2C bus

For `climate` (Exercise 2) and `climate_display` (Exercise 3).

## What you see

The Serial Monitor prints `climate started` or `climate_display started`, then nothing.
Reset and flashing don't help. Taking the USB power off does.

## Why

A device on the PicoBricks is cut off in the middle of a byte and keeps the data line SDA
low. It waits for more clock pulses. The Pico waits for a free bus before it sends any. So
both wait forever. Reset only restarts the Pico, not that device.

It is not known yet what sets it off. It doesn't happen very often.

## The fix

- `I2cBus` frees the bus when it starts: if SDA is held low, it sends up to 9 clock pulses.
- `write()` and `read()` give up after 10 ms per byte and return `false`.
- New `is_free()`: `false` if SDA or SCL is held low.
- The bus runs at 100 kHz, not 400 kHz.

## Why 100 kHz

kHz is how many times per second the clock line ticks. One tick carries one bit: 2.5 us
per bit at 400 kHz, 10 us at 100 kHz.

- A chip can only pull an I2C line low. A resistor pulls it back up, and that takes about
  0.3 us. At 400 kHz that is a big part of each bit.
- Each device needs time to answer. One that answers late can end up stuck in the middle of
  a byte. At 100 kHz it has four times as long.

The price: the screen takes about 100 ms to update, up from 25 ms.

## What you do

1. Copy `i2c_bus.h` and `i2c_bus.cpp` from [`code/climate/`](code/climate/) over the ones
   in your `climate` and `climate_display` projects.
2. In `climate.cpp` and `climate_display.cpp`, change the speed:

   ```cpp
   constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
   ```

3. In the same two files, add this below the line that makes `bus`:

   ```cpp
   if (!bus.is_free())
   {
       std::cout << "I2C bus is stuck. Take the power off the Pico and the PicoBricks, "
                 << "then put it back." << std::endl;
   }
   ```

4. Build and run. Your answers stay the same.

If the program hangs again: press reset. If you see `I2C bus is stuck`, unplug the USB
cables of the Pico and the PicoBricks, wait a few seconds, plug them in again.