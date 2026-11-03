#include <cstdint>
#include <iostream>

#include "pico/stdlib.h"

constexpr std::uint32_t REPEAT_MS {5000};
constexpr float TARGET_CELSIUS {21.0f};
constexpr float COLD_CELSIUS {18.0f};
constexpr float START_CELSIUS {20.0f};

// A pretend temperature sensor. set() gives it a value.
class FakeSensor
{
public:
    FakeSensor() { std::cout << "  FakeSensor made" << std::endl; }
    ~FakeSensor() { std::cout << "  FakeSensor gone" << std::endl; }

    float read() const { return celsius_; }
    void set(float celsius) { celsius_ = celsius; }

private:
    float celsius_ {START_CELSIUS};
};

// A pretend heater. It only remembers if it is on.
class Heater
{
public:
    Heater() { std::cout << "  Heater made" << std::endl; }
    ~Heater() { std::cout << "  Heater gone" << std::endl; }

    bool is_on() const { return on_; }
    void set(bool on) { on_ = on; }

private:
    bool on_ {false};
};

// IT variant: inheritance. This thermostat is a FakeSensor, and has a Heater.
class InheritingThermostat : public FakeSensor
{
public:
    explicit InheritingThermostat(float target) : target_ {target} {}

    void update() { heater_.set(read() < target_); }
    bool heating() const { return heater_.is_on(); }

private:
    float target_;
    Heater heater_;
};

// TODO 1: CT variant, a class ComposingThermostat. It is no FakeSensor. It has a Heater
//         (a member, like in the IT variant) and uses a FakeSensor it is given in the
//         constructor (a reference). Give it the same update() and heating() as the IT
//         variant.

int main()
{
    stdio_init_all();

    while (true)
    {
        std::cout << "-- IT: inheritance --" << std::endl;
        {
            InheritingThermostat it {TARGET_CELSIUS};
            it.set(COLD_CELSIUS);
            it.update();
            std::cout << "  it heating: " << (it.heating() ? "yes" : "no") << std::endl;
        }
        std::cout << "-- it is gone --" << std::endl;

        std::cout << "-- CT: composition and aggregation --" << std::endl;
        FakeSensor sensor;
        {
            // TODO 2: make a ComposingThermostat ct with TARGET_CELSIUS and sensor. Set sensor
            //         to COLD_CELSIUS, update ct and print if ct is heating, like for it.
        }
        std::cout << "-- ct is gone --" << std::endl;

        // TODO 3: print sensor.read(). The sensor is still there.

        std::cout << std::endl;
        sleep_ms(REPEAT_MS);
    }
}
