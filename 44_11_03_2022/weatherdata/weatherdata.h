#pragma once

#include "subject.h"
#include <vector>

// İmplementasyonları burada yapılacak. En azından override bildirimleri burada.
// Halen daha observerda değiliz.
class WeatherData : public ISubject {
public:
    void register_observer(IObserver* observer) override;
    void remove_observer(IObserver* observer) override;
    void notify_observers() override; 
    void set_state(double temp, double humidity, double pressure);
private:
    std::vector<IObserver*> m_observers; 
    double m_temp{};
    double m_humidity{};
    double m_pressure{};
};
