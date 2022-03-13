#pragma once

class IObserver;

class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void register_observer(IObserver* observer) = 0;
    virtual void remove_observer(IObserver* observer) = 0;
    virtual void notify_observers() = 0;
};