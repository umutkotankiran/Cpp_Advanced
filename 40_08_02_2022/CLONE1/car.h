#pragma once


#include <memory>
#include <string>
#include <iosfwd>

class Car {
public:
    Car() = default;

    Car(const std::string& licence, const std::string& color, int age) :
        m_licence(licence),
        m_color(color),
        m_age(age)
    {}
    virtual ~Car() = default;

    auto clone() const
    {
        return std::unique_ptr<Car>{do_clone()};
    }

    auto create() const
    {
        return std::unique_ptr<Car>{do_create()};
    }

    std::string get_licence()const { return m_licence; }
    std::string get_color()const { return m_color; }
    int get_age()const { return m_age; }


    friend std::ostream& operator<<(std::ostream& os, const Car& c)
    {
        return c.print(os);
    }
private:
    std::string m_licence{ "temporary" };
    std::string m_color{ "factory color" };
    int m_age{};
    virtual Car* do_clone() const = 0;
    virtual Car* do_create() const = 0;
    virtual std::ostream& print(std::ostream& os)const = 0;
};

