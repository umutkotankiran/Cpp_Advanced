#include "mercedes.h"
#include "volvo.h"
#include "bmw.h"
#include "audi.h"
#include <memory>

void g_collide(Car* p1, Car* p2)
{
	p1->collide(p2);
}


int main()
{
	g_collide(new Audi, new Volvo);
}


