#ifndef SENSOR_H
#define SENSOR_H

#include <string>

namespace hw
{
    // The base of every sensor. It is abstract: read() and unit() have no body here.
    class Sensor
    {
    public:
        virtual ~Sensor() = default;

        // not virtual: every sensor keeps its name the same way
        const std::string& name() const { return name_; }

        // virtual: each kind of sensor reads in its own way
        virtual float read() const = 0;
        virtual std::string unit() const = 0;

    protected:
        // protected: only a derived class can make a Sensor
        explicit Sensor(const std::string& name) : name_ {name} {}

    private:
        std::string name_;
    };
}

#endif  // SENSOR_H
