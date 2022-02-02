#pragma once

#include "document.h"

class XmlDocument : public Document {
public:
	XmlDocument();
	~XmlDocument();
	void save()override;
	void print()override;
};