#include "bird.h"
#include "petvisitor.h"

void Bird::accept(PetVisitor& v)
{
	v.visit(this);
}
