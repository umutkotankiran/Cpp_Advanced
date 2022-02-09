#pragma once
#include "builder.h"

class Product;

class ConcreteBuilder: public Builder
{
public:
	ConcreteBuilder();
	void build_part_a()override;
	void build_part_b()override;
	void build_part_c()override;
	Product * get_product();
private:
	Product* m_product{};
};

