#include "linux_textbox.h"
#include <iostream>

LinuxTextBox::LinuxTextBox()
{
	std::cout << "Linux Text Box created!\n";
}

LinuxTextBox::~LinuxTextBox()
{
	std::cout << "Linux Text Box destroyed!\n";
}

void LinuxTextBox::set_textbox_font()
{
	std::cout << "LINUX text box font set\n";
}

void LinuxTextBox::set_textbox_size()
{
	std::cout << "LINUX text box size set\n";
}
