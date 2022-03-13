#pragma once

class Cat;
class Dog;
class Bird;

class PetVisitor {
public:
	virtual void visit(Cat*) = 0;
	virtual void visit(Dog*) = 0;
	virtual void visit(Bird*) = 0;
};
