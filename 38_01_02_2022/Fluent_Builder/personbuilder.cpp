#include "person_builder.h"

PersonBuilder::PersonBuilder(std::string name) : m_name{ std::move(name) }
{ }

PersonBuilder& PersonBuilder::street(std::string street)
{
	m_street = std::move(street);
	return *this;
}

PersonBuilder& PersonBuilder::post_code(std::string post_code)
{
	m_post_code = std::move(post_code);
	return *this;
}

PersonBuilder& PersonBuilder::town(std::string town)
{
	m_town = std::move(town);
	return *this;
}

PersonBuilder& PersonBuilder::company(std::string company)
{
	m_company = std::move(company);
	return *this;
}

PersonBuilder& PersonBuilder::position(std::string position)
{
	m_position = std::move(position);
	return *this;
}

PersonBuilder& PersonBuilder::income(std::string income)
{
	m_income = std::move(income);
	return *this;
}

Person PersonBuilder::build()
{
	return Person{ m_name, m_town, m_street, m_post_code, m_company, m_position, m_income };
}


