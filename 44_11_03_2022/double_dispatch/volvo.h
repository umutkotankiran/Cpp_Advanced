#pragma once

#include "car.h"

class Volvo : public Car {
public:
	virtual void collide(Car* p) override;
	virtual void collide_with(Bmw* p) override;
	virtual void collide_with(Audi* p) override;
	virtual void collide_with(Mercedes* p) override;
	virtual void collide_with(Volvo* p) override;
};


