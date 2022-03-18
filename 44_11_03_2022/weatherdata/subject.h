#pragma once

class IObserver;

// Burası subject interface
// Durumu değiştiğinde kendi gözlemcilerine bunu haber verek olan sınıf demiştik.
// Bunlar pure virtual olmayabilirdi doğrudan burayada implementasyonda yapabilirdik.
class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void register_observer(IObserver* observer) = 0;
    virtual void remove_observer(IObserver* observer) = 0;
    virtual void notify_observers() = 0;
};
