#include "volvo.h"
#include "mercedes.h"
#include "audi.h"
#include "bmw.h"
#include <iostream>


void Volvo::collide(Car* p) 
{
	p->collide_with(this);
}

void Volvo::collide_with(Bmw* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Volvo::collide_with(Audi* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

	void Volvo::collide_with(Mercedes* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

	void Volvo::collide_with(Volvo* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}


