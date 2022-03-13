#pragma once

#include <string>

class PetVisitor;

class Pet {
public:
	virtual ~Pet() = default;
	virtual void accept(PetVisitor& v) = 0;
	Pet(const std::string& name, const std::string& color) : m_name{ name }, m_color(color) {}
	const std::string& name() const { return m_name; }
	const std::string& color() const { return m_color; }
private:
	std::string m_name;
	std::string m_color;
};
