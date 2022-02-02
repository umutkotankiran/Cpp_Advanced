#include "linux_button.h"
#include <iostream>

LinuxButton::LinuxButton()
{
	std::cout << "Linux Button created!\n";
}

LinuxButton::~LinuxButton()
{
	std::cout << "Linux Button destroyed!\n";
}

void LinuxButton::set_button_color()
{
	std::cout << "LINUX button color set!\n";
}

void LinuxButton::set_button_size()
{
	std::cout << "LINUX button size set!\n";
}

void LinuxButton::make_button_visible()
{
	std::cout << "LINUX button made visible!\n";
}

void LinuxButton::make_button_invisible()
{
	std::cout << "LINUX button made invisible!\n";
}
