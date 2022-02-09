#pragma once
#include "car.h"

class Fiat : public Car {
public:
    using Car::Car;
    auto clone() const { return std::unique_ptr<Fiat>{do_clone()}; }
    auto create() const { return std::unique_ptr<Fiat>{do_create()}; }
    std::ostream& print(std::ostream& os)const override;

private:
    Fiat* do_clone() const override;
    Fiat* do_create() const override;
};

