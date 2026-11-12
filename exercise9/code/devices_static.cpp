#include <cstdint>
#include <iostream>
#include <string>

#include "pico/stdlib.h"

constexpr std::uint32_t REPEAT_MS {5000};

// The base class from Session 8, as a class template. Nothing here is virtual.
// Derived is the class that derives from it: class Buzzer : public Device<Buzzer>.
template <typename Derived>
class Device
{
public:
    const std::string& name() const { return name_; }

    // Not handed on: like the not virtual describe() in Session 8
    void describe() const { std::cout << name_ << " describe(): a device" << std::endl; }

    // Handed on to Derived: this replaces the virtual self_test() from Session 8
    void self_test() const { static_cast<const Derived*>(this)->run_self_test(); }

protected:
    // Protected: only a derived class can make a Device
    explicit Device(const std::string& name) : name_ {name} {}

private:
    std::string name_;
};

// A Buzzer is a Device<Buzzer>. It has its own describe() and run_self_test().
class Buzzer : public Device<Buzzer>
{
public:
    Buzzer() : Device<Buzzer> {"Buzzer"} {}

    void describe() const { std::cout << name() << " describe(): makes a sound" << std::endl; }

    void run_self_test() const { std::cout << name() << " self_test(): beep" << std::endl; }
};

// TODO 1: a class Button that is a Device<Button>, named "Button". Give it its own
//         describe() ("reads a press") and run_self_test() ("press me"), like Buzzer.

// A function template: the compiler makes one check() for each Derived it is called with.
template <typename Derived>
void check(const Device<Derived>& device)
{
    device.describe();
    device.self_test();
}

int main()
{
    stdio_init_all();

    while (true)
    {
        const Buzzer buzzer;

        std::cout << "-- called on the object --" << std::endl;
        buzzer.describe();
        buzzer.self_test();
        // TODO 2: create a Button called button. Call describe() and self_test() on it.

        std::cout << "-- called through const Device<Derived>& --" << std::endl;
        check(buzzer);
        // TODO 3: check button too.

        std::cout << "sizeof: Buzzer " << sizeof(Buzzer) << " bytes" << std::endl;
        std::cout << std::endl;
        sleep_ms(REPEAT_MS);
    }
}
