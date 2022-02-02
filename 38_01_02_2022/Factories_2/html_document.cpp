#include "html_document.h"
#include <iostream>


HtmlDocument::HtmlDocument()
{
	std::cout << "HTML document created!\n";
}

HtmlDocument::~HtmlDocument()
{
	std::cout << "HTML document destroyed!\n";
}

void HtmlDocument::save()
{
	std::cout << "HTML document saved!\n";
}

void HtmlDocument::print()
{
	std::cout << "HTML document printed!\n";
}
