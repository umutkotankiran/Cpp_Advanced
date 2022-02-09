#include "concrete_builder.h"
#include "product.h" 
#include <iostream>

ConcreteBuilder::ConcreteBuilder()
{
	std::cout << "Concrete Builder object created\n" ;
}

void ConcreteBuilder::build_part_a()
{
	std::cout << "Concrete Builder is building part A\n";
}

void ConcreteBuilder::build_part_b()
{
	std::cout << "Concrete Builder is building part B\n";
}

void ConcreteBuilder::build_part_c()
{
	std::cout << "Concrete Builder is building part C\n";
}

Product* ConcreteBuilder::get_product() 
{
	m_product = new Product{}; 
	
	std::cout << "Concrete Builder returning the built object\n";
	return m_product;
}
