#include "nec.h"
#include <iostream>


Nec::Nec(int x) : m_val{x}
{
	std::cout << "Nec object with value " << m_val << " constructed\n";
}

Nec::~Nec()
{
	std::cout << "Nec object with value " << m_val << " destroyed\n";
}


std::ostream& operator<<(std::ostream& os, const Nec& nec)
{
	return os << '(' << nec.m_val << ')';
}
