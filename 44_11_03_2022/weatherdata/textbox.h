#pragma once

#include "observer.h"

class TextBox : public IObserver {
public:
    void update(double temp, double humidity, double pressure) override;
};
