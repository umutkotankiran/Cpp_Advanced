#include "fiat.h"
#include <ostream>


Fiat* Fiat::do_clone() const
{
    return new Fiat{ *this };
}

Fiat* Fiat::do_create() const
{
    return new Fiat;
}

std::ostream& Fiat::print(std::ostream& os)const
{
    return os << "Fiat car licence no: [" << get_licence() << "] color : [" << get_color() << "] age : " << get_age();
}


