#pragma once

#include <string>
#include <typeinfo>

class Bmw;
class Mercedes;
class Audi;
class Volvo;

class Car {
public:
	virtual void collide(Car*) = 0;
	virtual void collide_with(Bmw*) {}
	virtual void collide_with(Mercedes*) {}
	virtual void collide_with(Audi*) {};
	virtual void collide_with(Volvo*) {};
	virtual std::string name()const
	{
		return typeid(*this).name();
	}
};
