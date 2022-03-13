#include "playing_visitor.h"
#include <iostream>
#include <format>
#include "cat.h"
#include "dog.h"
#include "bird.h"

void PlayingVisitor::visit(Cat* cp)
{
	std::cout << std::format("{} isimli {} renkli kedi lazer fenerle oynatiliyor", cp->name(), cp->color()) << '\n';

}

void PlayingVisitor::visit(Dog* dp)
{
	std::cout << std::format("{} isimli {} renkli kopek top ile oynatiliyor", dp->name(), dp->color()) << '\n';
}

void PlayingVisitor::visit(Bird* bp)
{
	std::cout << std::format("{} isimli {} renkli kus ip ile oynatiliyor", bp->name(), bp->color()) << '\n';
}
