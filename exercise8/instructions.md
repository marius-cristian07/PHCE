# Exercise Session 8

Work through the exercises in order. Write answers in the `_Answer:_` blocks. Commit this
file and your `.cpp` and `.h` files.

Exercises 1-3 are for the session. Exercises 4 and 5 are homework.
See [`homework.md`](homework.md).

---

## Exercise 1: A family of sensors

**Goal:** Read the pot, the light sensor, the chip's temperature and the button through
one base class, `Sensor`.

### Instructions

1. Create a project `sensors`. Copy all files from [`code/sensors/`](code/sensors/) into it.
   `sensors.cpp` replaces the generated one.
2. In `CMakeLists.txt`, add `adc_sensor.cpp`, `analog_sensors.cpp` and `button.cpp` to
   `add_executable(sensors ...)`. Add `hardware_adc` to `target_link_libraries`.
3. Read `sensor.h`, `adc_sensor.h`, `analog_sensors.h` and `button.h`. Find what each class
   adds.
4. Read `Potentiometer::read()` in `analog_sensors.cpp`. Fill in TODO 1 the same way. Less
   light gives a higher raw value, so turn it around.
5. Fill in TODO 2 in `sensors.cpp`, in `print_sensor()`.
6. Fill in TODO 3 in `sensors.cpp`. `sensors` holds pointers: use `*sensor` to get the
   object.
7. Open the Serial Monitor, build and run. Turn the knob. Cover the light sensor. Press
   the button.
8. In `analog_sensors.h`, remove `const` from `float read() const override;` in
   `LightSensor`. Build and read the **first** error. Put `const` back.

### Checklist

- [x] TODO 1-3 filled in; all four sensors print
- [x] The light value goes down when you cover the sensor
- [x] Error read (step 8)

**`Potentiometer` has no `name()`, but `sensor.name()` works for it. Where does it come
from? Which functions does `Potentiometer` write itself?**

> _Answer: name() is inherited from the base class Sensor, through AdcSensor. The Potentiometer class implements its own constructor, read(), and unit() functions. It inherits name() from Sensor and read_raw() from AdcSensor_
>

**`print_sensor()` only knows `const hw::Sensor&`. How does it call the right `read()`?
Could it call `read_raw()`? Why not?**

> _Answer: It uses runtime polymorphism. Since read() is a virtual function in Sensor, C++ automatically calls the correct implementation depending on the actual sensor object. It cannot call read_raw() because that function is protected in AdcSensor and is not part of the public Sensor interface._
>

**What was the first error in step 8? Without the word `override`, what would the
compiler have said, and where?**

> _Answer: The error was that LightSensor::read() was marked override but did not override the base class function because const was missing. Without override, the compiler would not detect that mismatch at the declaration. It would instead report errors elsewhere, such as the definition in analog_sensors.cpp not matching the declaration and LightSensor remaining an abstract class when instantiated in main()._
>

**`Button` is not an `AdcSensor`. What did `main()` need to print it with the others? Did
`print_sensor()` change?**

> _Answer: main() only needed to create a Button object and add its pointer to the Sensor pointer array. print_sensor() did not need any changes because Button inherits from Sensor and implements the same virtual functions, read() and unit(). This demonstrates polymorphism._
>

**Attached file(s):**

> _Filename: Exercise1_
>

*Read more (optional): Beginning C++17, Chapter 13 ("Inheritance in Classes", "Deriving
Classes", "protected Members of a Class", "Constructors in a Derived Class"), and Chapter
14 ("Using a Base Class Pointer", "Virtual Functions", "Using override", "Pure Virtual
Functions", "Abstract Classes"). Real-Time C++, Sect. 4.3 ("Inheritance"), pp. 67-68,
Sect. 4.4 ("Dynamic Polymorphism"), pp. 69-70, and Sect. 4.6 ("Pure Virtual and
Abstract"), pp. 71-72.*

---

## Exercise 2: The SHTC3 over I2C

**Goal:** Read temperature and humidity from the SHTC3, with the I2C bus handed to it.

### Instructions

1. Create a project `climate`. Copy all files from [`code/climate/`](code/climate/) into
   it. `climate.cpp` replaces the generated one.
2. In `CMakeLists.txt`, add `i2c_bus.cpp` and `shtc3.cpp` to `add_executable(climate ...)`.
   Add `hardware_i2c` to `target_link_libraries`.
3. Read `i2c_bus.h` and `shtc3.h`. Find the constructor of `Shtc3` and the member `bus_`.
   In `climate.cpp`, find where the bus goes into the sensor.
4. Open the Serial Monitor, build and run. `scan()` prints the address of each device.
   Write them down.
5. Fill in TODO 1 in `shtc3.cpp`: `bus_.read(ADDRESS, result.data(), result.size())`.
6. Fill in TODO 2 in `shtc3.cpp`: `T = -45 + 175 * raw / FULL_SCALE` (°C) and
   `RH = 100 * raw / FULL_SCALE` (%).
7. Fill in TODO 3 in `climate.cpp`. Build and run. Breathe on the sensor.
8. In `shtc3.h`, change `ADDRESS` to `0x71`. Build and run. Put `0x70` back.

### Checklist

- [x] TODO 1-3 filled in
- [x] The scan shows the devices
- [x] Temperature and humidity print; breath makes the humidity go up
- [x] Wrong address tried (step 8)

**Which addresses did the scan find? Which one is the SHTC3? What could the other one
be?**

> _Answer: The expected addresses include 0x70 for the SHTC3 temperature and humidity sensor and possibly 0x3C or 0x3D for the OLED display. Both devices communicate through the same I2C bus. The exact addresses found depend on the Serial Monitor output._
>

**What did step 8 print? Which function reported the problem?**

> _Answer: Step 8 printed "measure failed" because the sensor address was changed from 0x70 to 0x71. The I2cBus::write() function detected the communication failure and returned false. This caused send_command() and then measure() to return false, which made main() print the error message._
>

**Why does `main()` make the bus, and not `Shtc3`'s constructor? What goes wrong if each
device makes its own `I2cBus`?**

> _Answer: main() creates the I2C bus so multiple devices can share the same connection. The Shtc3 constructor receives a reference to that bus instead of creating a new one. If every device created its own I2cBus, they could repeatedly initialize the same hardware controller and pins, causing conflicts. Sharing one bus avoids this problem and makes the code more efficient and organized._
>

**Attached file(s):**

> _Filename: Exercise2_
>

*Read more (optional): Beginning C++17, Chapter 13 ("Inheritance vs. Aggregation"), and
Chapter 11 ("Using a Member Initializer List"). Real-Time C++, Sect. 4.7 ("Class
Relationships"), pp. 72-74.*

---

## Exercise 3: Text on the OLED

**Goal:** Show the temperature and humidity on the OLED, with the same bus.

### Instructions

1. Create a project `climate_display`. Copy all files from
   [`code/climate_display/`](code/climate_display/) into it. `climate_display.cpp` replaces
   the generated one. Copy your `i2c_bus.h`, `i2c_bus.cpp`, `shtc3.h` and `shtc3.cpp` from
   Exercise 2. Don't copy `climate.cpp`.
2. In `CMakeLists.txt`, add `i2c_bus.cpp`, `shtc3.cpp` and `ssd1306.cpp` to
   `add_executable(climate_display ...)`. Add `hardware_i2c` to `target_link_libraries`.
3. Read `ssd1306.h`. Compare its constructor and its `bus_` with `Shtc3`. In
   `climate_display.cpp`, find where the bus goes into both devices.
4. Fill in TODO 1 in `ssd1306.cpp`, in `show()`. Send all of `buffer_` with one
   `bus_.write()`, like `Shtc3::send_command()`.
5. Fill in TODO 2 in `climate_display.cpp`.
6. Open the Serial Monitor, build and run. Breathe on the sensor and watch the OLED.

### Checklist

- [x] TODO 1 and 2 filled in; the OLED shows temperature and humidity

**Comment out `display.show()` and run. What does the screen show? Why do drawing and
sending happen in two steps?**

> _Answer: The screen stays blank or shows the previous image because draw_text() only modifies the memory buffer, while display.show() sends that buffer to the OLED. These operations are separated so we can prepare the entire image before updating the screen, reducing unnecessary I2C communication and avoiding partially updated images._
>

**`Shtc3` and `Ssd1306` both hold an `I2cBus&`. Should they share a base class, like the
sensors in Exercise 1? Should `Ssd1306` derive from `Sensor`? Why?**

> _Answer: No, they shouldn't share a base class just because they both use I2C. They have different responsibilities: Shtc3 measures temperature and humidity, while Ssd1306 displays information. Ssd1306 should not inherit from Sensor because it isn't a sensor and doesn't implement the read() and unit() functions. Sharing a bus is not a reason for inheritance._
>

**`Ssd1306` owns `buffer_` and only uses `bus_`. Which one is composition, which one is
aggregation?**

> _Answer: buffer_ represents composition because it belongs directly to Ssd1306 and its lifetime is tied to the display object. bus_ represents aggregation because Ssd1306 only holds a reference to an existing I2cBus object created in main(). The bus exists independently and can be shared with other devices._
>

**Attached file(s):**

> _Filename: Exercise3_
>

*Read more (optional): Beginning C++17, Chapter 13 ("Inheritance vs. Aggregation").
Real-Time C++, Sect. 4.7 ("Class Relationships"), pp. 72-74: is-a, has-a and uses-a.*

Exercises 4 and 5 continue in [`homework.md`](homework.md).
