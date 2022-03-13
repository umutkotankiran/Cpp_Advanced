#include "audi.h"
#include "mercedes.h"
#include "volvo.h"
#include "bmw.h"
#include <iostream>


void Audi::collide(Car* p) 
{
	p->collide_with(this);
}

void Audi::collide_with(Bmw* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Audi::collide_with(Audi* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Audi::collide_with(Mercedes* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Audi::collide_with(Volvo* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}


