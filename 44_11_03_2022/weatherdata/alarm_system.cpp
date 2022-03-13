#include "alarm_system.h"
#include <iostream>


void AlarmSystem::update(double temp, double humidity, double pressure)
{
	if (temp > 60.) {
		std::cout << "Alarm : very high temperature!!!\n";
	}

	if (humidity > 5.) {
		std::cout << "Alarm : very high humidity!!!\n";
	}

	if (pressure > 4.) {
		std::cout << "Alarm : very high weather pressure!!!\n";
	}
	std::cout << "-----------------------------------------------------------------------------------\n";

}
