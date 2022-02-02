#include "xml_document.h"
#include <iostream>

XmlDocument::XmlDocument()
{
	std::cout << "XML document created!\n";
}

XmlDocument::~XmlDocument()
{
	std::cout << "XML document destroyed!\n";
}

void XmlDocument::save()
{
	std::cout << "XML document saved!\n";
}

void XmlDocument::print()
{
	std::cout << "XML document printed!\n";
}
