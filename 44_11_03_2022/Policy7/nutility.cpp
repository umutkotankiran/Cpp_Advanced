#include "nutility.h"
#include "necheap.h"


void* operator new(size_t size, Heap* hp)
{
	return hp->allocate(size);
}