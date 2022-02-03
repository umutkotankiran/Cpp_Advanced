#pragma once
#include "person.h"

class PersonBuilder {
public:
	PersonBuilder(std::string name);
	PersonBuilder& street(std::string street);
	PersonBuilder& post_code(std::string post_code);
	PersonBuilder& town(std::string town);
	PersonBuilder& company(std::string company);
	PersonBuilder& position(std::string position);
	PersonBuilder& income(std::string income);
	Person build();
private:
	std::string m_name;
	std::string m_street;
	std::string m_post_code;
	std::string m_town;
	std::string m_company;
	std::string m_position;
	std::string m_income;
};