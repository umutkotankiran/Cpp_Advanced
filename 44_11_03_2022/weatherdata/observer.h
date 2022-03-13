#pragma once

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(double temp, double humidity, double pressure) = 0;
};