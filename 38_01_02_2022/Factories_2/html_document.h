#pragma once

#include "document.h"

class HtmlDocument : public Document {
public:
	HtmlDocument();
	~HtmlDocument();
	void save()override;
	void print()override;
};
