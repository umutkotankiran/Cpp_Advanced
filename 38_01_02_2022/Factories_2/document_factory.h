#pragma once

#include <memory>
#include <string_view>
#include "document.h"

class DocumentFactory {
public:
	static DocumentPtr create(std::string_view);
};
