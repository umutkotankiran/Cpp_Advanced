#include <iostream>
#include "person.h"
#include "person_builder.h"


int main()
{
	auto p = PersonBuilder{ "necati ergin" }
		.company("CSD")
		.position("developer")
		.town("istanbul")
		.street("beykent")
		.post_code("34576")
		.income("5'000 TL").build();

	std::cout << p;
}