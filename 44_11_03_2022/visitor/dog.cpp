#include "dog.h"
#include "petvisitor.h"

void Dog::accept(PetVisitor& v)
{
	v.visit(this);
}
