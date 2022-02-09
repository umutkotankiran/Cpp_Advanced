#pragma once
#include "fiat.h"

class Fiat124 : public Fiat {
public:
    using Fiat::Fiat;
    Fiat124() = default;
    auto clone() const { return std::unique_ptr<Fiat124>{do_clone()}; }
    auto create() const { return std::unique_ptr<Fiat124>{do_create()}; }
    std::ostream& print(std::ostream& os)const override;

private:
    Fiat124* do_clone() const override;
    Fiat124* do_create() const override;
};




