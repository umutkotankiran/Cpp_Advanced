#pragma once
#include "pet.h"

class Cat : public Pet {
public:
	using Pet::Pet;
	virtual void accept(PetVisitor& v) override;
};
