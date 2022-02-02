#include "windows_button.h"
#include <iostream>

WindowsButton::WindowsButton()
{
	std::cout << "Windowes Button created!\n";
}

WindowsButton::~WindowsButton()
{
	std::cout << "Windowes Button destroyed!\n";
}

void WindowsButton::set_button_color()
{
	std::cout << "WINDOWS button color set!\n";
}

void WindowsButton::set_button_size()
{
	std::cout << "WINDOWS button size set!\n";
}

void WindowsButton::make_button_visible()
{
	std::cout << "WINDOWS button made visible!\n";
}

void WindowsButton::make_button_invisible()
{
	std::cout << "WINDOWS button made invisible!\n";
}

