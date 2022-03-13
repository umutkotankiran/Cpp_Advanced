#include "cat.h"
#include "petvisitor.h"

void Cat::accept(PetVisitor& v)
{
	v.visit(this);
}
