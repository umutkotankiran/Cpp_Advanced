#include "linux_menu.h"
#include <iostream>

LinuxMenu::LinuxMenu()
{
	std::cout << "Linux Menu created!\n";
}

LinuxMenu::~LinuxMenu()
{
	std::cout << "Linux Menu destroyed!\n";
}

void LinuxMenu::set_menu_size()
{
	std::cout << "LINUX menu size set!\n";
}

void LinuxMenu::set_menu_text()
{
	std::cout << "LINUX menu text set!\n";
}

void LinuxMenu::set_menu_items()
{
	std::cout << "LINUX menu iterm set!\n";
}
