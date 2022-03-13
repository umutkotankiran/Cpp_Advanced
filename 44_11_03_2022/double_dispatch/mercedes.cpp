#include "mercedes.h"
#include "bmw.h"
#include "audi.h"
#include "volvo.h"
#include <iostream>

void Mercedes::collide(Car* p) 
{
	p->collide_with(this);
}

void Mercedes::collide_with(Bmw* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Mercedes::collide_with(Audi* p) 
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Mercedes::collide_with(Mercedes* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Mercedes::collide_with(Volvo * p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

