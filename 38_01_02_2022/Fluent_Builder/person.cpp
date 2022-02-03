#include <iostream>
#include <format>
#include "person.h"
#include "person_builder.h"



std::ostream & operator<<(std::ostream & os, const Person & per)
{
	return os << std::format("{} {} ilinde posta kodu {} olan {} caddesinde oturuyor. {} sirketinde {} olarak {} maaþ alýyor.", per.m_name, per.m_town, per.m_post_code, 
		per.m_street, per.m_company, per.m_position, per.m_income);
}