#include "textbox.h"
#include <iostream>

void TextBox::update(double temp, double humidity, double pressure)
{
	std::cout << "new values displayed in text box\n";
	std::cout << "temperature : " << temp << " celcius\n";
	std::cout << "humidity    : " << humidity << " gr / m3\n";
	std::cout << "pressure    : " << pressure << " bar\n";
	std::cout << "-----------------------------------------------------------------------------------\n";
}
