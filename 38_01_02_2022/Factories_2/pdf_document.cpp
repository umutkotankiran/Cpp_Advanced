#include "pdf_document.h"
#include <iostream>

PdfDocument::PdfDocument()
{
	std::cout << "PDF document created!\n";
}

PdfDocument::~PdfDocument()
{
	std::cout << "PDF document destroyed!\n";
}

void PdfDocument::save()
{
	std::cout << "PDF document saved!\n";
}

void PdfDocument::print()
{
	std::cout << "PDF document printed!\n";
}
