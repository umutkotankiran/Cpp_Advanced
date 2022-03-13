#pragma once
#include "bmw.h"
#include "mercedes.h"
#include "audi.h"
#include "volvo.h"
#include <iostream>

void Bmw::collide(Car* p) 
{
	p->collide_with(this);
}

void Bmw::collide_with(Bmw* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Bmw::collide_with(Audi* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Bmw::collide_with(Mercedes* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}

void Bmw::collide_with(Volvo* p)
{
	std::cout << name() << " collides with " << p->name() << '\n';
}


