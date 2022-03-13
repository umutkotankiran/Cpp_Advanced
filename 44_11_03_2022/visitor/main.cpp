#include "cat.h"
#include "dog.h"
#include "bird.h"
#include "feeding_visitor.h"
#include "playing_visitor.h"


void func(Pet*pet, PetVisitor &visitor)
{
	pet->accept(visitor);
}

int main()
{
	Pet* p1 = new Cat("pammuk", "beyaz");
	Pet* p2 = new Dog("kont", "gri");
	Pet* p3 = new Bird("geveze", "sari");
	FeedingVisitor fv;
	PlayingVisitor pv;

	func(p2, fv);
	//p2->accept(fv);
	//p3->accept(fv);
	//p1->accept(pv);
	//p2->accept(pv);
	//p3->accept(pv);
}





