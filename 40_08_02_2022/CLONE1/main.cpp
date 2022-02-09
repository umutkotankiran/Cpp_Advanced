#include "fiat124.h"
#include <memory>
#include <iostream>

using CarPtr = std::unique_ptr<Car>;

int main()
{
	auto p1 = new Fiat124{ "26 ar 1987", "red", 28 };
	auto p2 = p1->clone();
	//std::unique_ptr<Fiat124> p2 = p1->clone();
	std::cout << *p1 << '\n';
	std::cout << *p2 << '\n';

	CarPtr pc = p1->clone();
	std::cout << *pc << '\n';


}
