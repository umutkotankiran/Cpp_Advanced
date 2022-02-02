#include "ios_textbox.h"
#include <iostream>


IosTextBox::IosTextBox()
{
	std::cout << "IOS Text Box created!\n";
}

IosTextBox::~IosTextBox()
{
	std::cout << "IOS Text Box destroyed!\n";
}

void IosTextBox::set_textbox_font()
{
	std::cout << "IOS text box font set!\n";
}

void IosTextBox::set_textbox_size()
{
	std::cout << "IOS text box size set!\n";
}
