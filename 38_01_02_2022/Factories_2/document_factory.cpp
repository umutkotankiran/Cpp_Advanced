#include "document_factory.h"
#include "html_document.h"
#include "pdf_document.h"
#include "xml_document.h"

DocumentPtr DocumentFactory::create(std::string_view type)
{
    if (type == "html") {
        return std::make_unique<HtmlDocument>();
    }

    if (type == "pdf") {
        return std::make_unique<PdfDocument>();
    }

    if (type == "xml") {
        return std::make_unique<XmlDocument>();
    }

    return {};
}
