#pragma once

#pragma once

#include "observer.h"

class AlarmSystem : public IObserver {
public:
    void update(double temp, double humidity, double pressure) override;
};
