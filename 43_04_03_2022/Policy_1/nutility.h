#pragma once

class Heap;

void* operator new(size_t size, Heap* hp);	// placement new için bir overload yazdık.