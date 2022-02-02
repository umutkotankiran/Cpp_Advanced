#pragma once

#include "document.h"

class PdfDocument : public Document {
public:
	PdfDocument();
	~PdfDocument();
	void save()override;
	void print()override;
};