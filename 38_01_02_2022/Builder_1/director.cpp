#include "director.h"
#include "builder.h"
#include <iostream>

Director::Director(Builder* p_builder): m_pbuilder{p_builder} 
{
	std::cout << "director object created\n" ;
}

void Director::construct() 
{
	std::cout << "construction process of director has started\n" ;
	m_pbuilder->build_part_a();
	m_pbuilder->build_part_b();
	m_pbuilder->build_part_c();
}
