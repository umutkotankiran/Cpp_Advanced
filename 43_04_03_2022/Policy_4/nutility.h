#pragma once

class Heap;

void* operator new(size_t size, Heap* hp);
