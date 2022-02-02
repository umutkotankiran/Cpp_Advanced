#include "ios_menu.h"
#include <iostream>

IosMenu::IosMenu()
{
	std::cout << "IOS Menu created!\n";
}

IosMenu::~IosMenu()
{
	std::cout << "IOS Menu destroyed\n";
}

void IosMenu::set_menu_size()
{
	std::cout << "IOS menu size set!\n";
}

void IosMenu::set_menu_text()
{
	std::cout << "IOS menu text set!\n";
}

void IosMenu::set_menu_items()
{
	std::cout << "IOS menu iterm set!\n";
}
