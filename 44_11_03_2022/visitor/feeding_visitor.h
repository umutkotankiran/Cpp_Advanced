#pragma once
#include "petvisitor.h"
#include <iostream>


class FeedingVisitor : public PetVisitor {
public:
	void visit(Cat*) override;
	void visit(Dog*) override;
	void visit(Bird*) override;
};
