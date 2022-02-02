#pragma once
#include <memory>
#include <string_view>
#include "document.h"

class Application {
public:
	void NewDocument(std::string_view);
	void SaveDocument();
	void PrintDocument();
private:
	void createDocument(std::string_view);
	DocumentPtr mptr{};
};