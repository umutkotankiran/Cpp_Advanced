#include "concrete_builder.h"
#include "director.h"
#include "product.h"

int main() 
{
	ConcreteBuilder builder;
	Director director{&builder};
	
	director.construct();
	Product *p = builder.get_product();
	
	delete p ;
}
