#include "weatherdata.h"

#include "observer.h"
#include <algorithm>
#include <stdexcept>


void WeatherData::register_observer(IObserver* observer) 
{
    m_observers.push_back(observer);
}

void WeatherData::remove_observer(IObserver* observer) 
{
    if (!std::erase(m_observers, observer)) {
        throw std::runtime_error{ "observer cannot be removed!" };
    }
}

void WeatherData::notify_observers()
{
    for (auto p : m_observers) { 
        p->update(m_temp, m_humidity, m_pressure);
    }
}

void WeatherData::set_state(double temp, double humidity, double pressure) 
{
    m_temp = temp;
    m_humidity = humidity;
    m_pressure = pressure;
    notify_observers();
}