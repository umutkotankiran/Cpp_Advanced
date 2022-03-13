#pragma once

#include "pet.h"

class Bird : public Pet {
public:
	using Pet::Pet; //inherited ctor
	virtual void accept(PetVisitor& v) override;
};

