#include "fiat124.h"
#include <ostream>

Fiat124* Fiat124::do_clone() const
{
    return new Fiat124{ *this };
}

Fiat124* Fiat124::do_create() const
{
    return new Fiat124;
}

std::ostream& Fiat124::print(std::ostream& os)const
{
    return os << "Fiat124 car licence no: [" << get_licence() << "] color : [" << get_color() << "] age : " << get_age();
}
