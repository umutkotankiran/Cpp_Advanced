#include "ios_button.h"
#include <iostream>


IosButton::IosButton()
{
	std::cout << "IOS Button created...\n";
}

IosButton::~IosButton()
{
	std::cout << "IOS Button destroyed\n";
}

void IosButton::set_button_color()
{
	std::cout << "IOS button color set!\n";
}

void IosButton::set_button_size()
{
	std::cout << "IOS button size set!\n";
}

void IosButton::make_button_visible()
{
	std::cout << "IOS button made visible!\n";
}

void IosButton::make_button_invisible()
{
	std::cout << "IOS button made invisible!\n";
}
