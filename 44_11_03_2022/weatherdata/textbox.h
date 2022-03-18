#pragma once
#include "observer.h"

//Concrete observerlar bunlar
class TextBox : public IObserver {
public:
    void update(double temp, double humidity, double pressure) override;
};
