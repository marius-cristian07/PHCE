#ifndef RESULT_H
#define RESULT_H

#include <cstdint>
#include <string_view>

namespace hw
{
    // What can go wrong when the program reads a sensor
    enum class Error : std::uint8_t
    {
        BusStuck,  // a device holds a line of the bus low
        NoAnswer   // the sensor did not answer
    };

    constexpr std::string_view to_string(Error error)
    {
        switch (error)
        {
            case Error::BusStuck:
                return {"bus stuck"};
            case Error::NoAnswer:
                return {"no answer"};
        }
        return {"unknown"};
    }

    // What a function gives back when it can fail: a value of type T, or an error of
    // type E. Never both.
    template <typename T, typename E>
    class Result
    {
    public:
        // Not explicit on purpose: a function can write return value; or return error;
        Result(const T& value) : value_ {value}, ok_ {true} {}
        Result(const E& error) : error_ {error}, ok_ {false} {}

        // true: there is a value. false: there is an error.
        bool ok() const { return ok_; }

        const T& value() const { return value_; }  // only if ok()
        const E& error() const { return error_; }  // only if not ok()

    private:
        T value_ {};
        E error_ {};
        bool ok_;
    };
}  // namespace hw

#endif  // RESULT_H
