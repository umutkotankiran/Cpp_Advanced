#pragma once
#include "pet.h"

class Dog : public Pet {
public:
	using Pet::Pet;
	virtual void accept(PetVisitor& v) override;
};
