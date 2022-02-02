#include "application.h"
#include "document_factory.h"

void Application::createDocument(std::string_view type)
{
	mptr = DocumentFactory::create(type);
	//
}

void Application::NewDocument(std::string_view type)
{
	createDocument(type);
}

void Application::SaveDocument()
{
	mptr->save();
}

void Application::PrintDocument()
{
	mptr->print();
}
