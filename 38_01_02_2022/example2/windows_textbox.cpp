#include "windows_textbox.h"
#include <iostream>


WindowsTextBox::WindowsTextBox()
{
	std::cout << "Windows Text Box created!\n";
}

WindowsTextBox::~WindowsTextBox()
{
	std::cout << "Windows Text Box destroyed!\n";
}

void WindowsTextBox::set_textbox_font()
{
	std::cout << "WINDOWS text box font set!\n";
}

void WindowsTextBox::set_textbox_size()
{
	std::cout << "WINDOWS text box size set!\n";
}
