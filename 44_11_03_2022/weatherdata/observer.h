#pragma once

// Observer yani gözlemci interfacei de burada.
// Bundan sonrada concrete observerları oluşturacağız.
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(double temp, double humidity, double pressure) = 0;
};
