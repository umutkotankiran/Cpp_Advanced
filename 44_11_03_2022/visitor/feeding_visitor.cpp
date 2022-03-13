#include "feeding_visitor.h"
#include <iostream>
#include <format>
#include "cat.h"
#include "dog.h"
#include "bird.h"


void FeedingVisitor::visit(Cat* cp) 	
{
	std::cout << std::format("{} isimli {} renkli kediye balik veriliyor", cp->name(), cp->color()) << '\n';
}

void FeedingVisitor::visit (Dog* dp)
{
	std::cout << std::format("{} isimli {} renkli kopege kemik veriliyor", dp->name(), dp->color()) << '\n';
}

void FeedingVisitor::visit(Bird* bp)
{
	std::cout << std::format("{} isimli {} renkli kusa kus yemi veriliyor", bp->name(), bp->color()) << '\n';
}