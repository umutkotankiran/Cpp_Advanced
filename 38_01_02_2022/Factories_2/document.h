#pragma once

#include <memory>

class Document {
public:
	virtual ~Document() = default;
	virtual void save() = 0;
	virtual void print() = 0;
};

using DocumentPtr = std::unique_ptr<Document>;