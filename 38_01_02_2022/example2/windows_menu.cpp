#include "windows_menu.h"
#include <iostream>

WindowsMenu::WindowsMenu()
{
	std::cout << "Windows Menu created!\n";
}

WindowsMenu::~WindowsMenu()
{
	std::cout << "Windows Menu destroyed!\n";
}

void WindowsMenu::set_menu_size()
{
	std::cout << "WINDOWS menu size set!\n";
}

void WindowsMenu::set_menu_text()
{
	std::cout << "WINDOWS menu text set!\n";
}

void WindowsMenu::set_menu_items()
{
	std::cout << "WINDOWS menu iterm set!\n";
}