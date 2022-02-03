#pragma once

#include <ostream>
#include <string>

class PersonBuilder;

class Person {
	std::string m_name;
	std::string m_town;
	std::string m_street;
	std::string m_post_code;
	std::string m_company;
	std::string m_position;
	std::string m_income;
	Person(std::string name,
		std::string town,
		std::string street,
		std::string post_code,
		std::string company,
		std::string position,
		std::string income) :
		m_name(std::move(name)),
		m_town(std::move(town)),
		m_street(std::move(street)),
		m_post_code(std::move(post_code)),
		m_company(std::move(company)),
		m_position(std::move(position)),
		m_income(std::move(income)) {}

public:
	friend class PersonBuilder;
	friend std::ostream & operator<<(std::ostream & os, const Person & obj);
};