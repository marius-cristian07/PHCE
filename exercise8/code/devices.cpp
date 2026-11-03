#include <cstdint>
#include <iostream>
#include <string>

#include "pico/stdlib.h"

constexpr std::uint32_t REPEAT_MS {5000};

// The base class. Every derived class inherits name(), describe() and self_test().
class Device
{
public:
    explicit Device(const std::string& name) : name_ {name} {}
    virtual ~Device() = default;

    const std::string& name() const { return name_; }

    // not virtual
    void describe() const { std::cout << name_ << " describe(): a device" << std::endl; }

    // virtual
    virtual void self_test() const { std::cout << name_ << " self_test(): no test" << std::endl; }

private:
    std::string name_;
};

// A Buzzer is a Device. It has its own describe() and self_test().
class Buzzer : public Device
{
public:
    Buzzer() : Device {"Buzzer"} {}

    void describe() const { std::cout << name() << " describe(): makes a sound" << std::endl; }

    void self_test() const override { std::cout << name() << " self_test(): beep" << std::endl; }
};

// TODO 1: a class Button that is a Device, named "Button". Give it its own describe()
//         ("reads a press") and self_test() ("press me"), like Buzzer.

// Sees every device as a Device.
void check(const Device& device)
{
    device.describe();
    device.self_test();
}

int main()
{
    stdio_init_all();

    while (true)
    {
        const Device device {"Device"};
        const Buzzer buzzer;

        std::cout << "-- called on the object --" << std::endl;
        device.describe();
        device.self_test();
        buzzer.describe();
        buzzer.self_test();
        // TODO 2: create a Button called button. Call describe() and self_test() on it.

        std::cout << "-- called through const Device& --" << std::endl;
        check(device);
        check(buzzer);
        // TODO 3: check button the same way.

        std::cout << std::endl;
        sleep_ms(REPEAT_MS);
    }
}
