#include "slider.h"
#include <string>
#include <iostream>


void Slider::update(double temp, double humidity, double pressure)
{
	std::cout << "temperature : " << std::string(static_cast<int>(std::round(temp * 1.5)), '*') << '\n';
	std::cout << "humidity    : " << std::string(static_cast<int>(std::round(humidity * 10.)), '*') << '\n';
	std::cout << "pressure    : " << std::string(static_cast<int>(std::round(pressure * 50.)), '*') << '\n';
	std::cout << "-----------------------------------------------------------------------------------\n";
}
