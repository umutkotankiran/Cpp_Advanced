#pragma once

#pragma once

#include "petvisitor.h"
#include <iostream>

class PlayingVisitor : public PetVisitor {
public:
	void visit(Cat*) override;
	void visit(Dog*) override;
	void visit(Bird*) override;
};