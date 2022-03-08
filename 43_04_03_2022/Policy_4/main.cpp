//template template parameter

#include "necheap.h"
#include "nec.h"
#include "scoped_ptr.h"
#include <iostream>

template <typename T>
struct FreeDeleter {
    void operator()(T* p) const
    {
        if (p) {
            p->~T();
            free(p);
        }
    }
};

template <typename T>
struct DestroyOnly
{
    void operator()(T* p) const
    {
        p->~T();
    }
};



template <typename T>
struct HeapDeleter{
    explicit HeapDeleter(Heap& heap) : m_heap(heap) {}
    void operator()(T* p) const
    {
        p->~T();
        m_heap.deallocate(p);
    }
private:
    Heap& m_heap;
};

using fdel_int = void (*)(int*);

void delete_int(int* p)
{
    delete p;
}

template <typename T>
void t_delete(T* p)
{
    delete p;
}

void f1()
{
    //ScopedPtr<int> sp(new int{ 34 });
    ScopedPtr sp(new int{ 34 });
    std::cout << "in function f1 *sp = " << *sp << "\n\n";
}

void f2()
{
    Heap h;
    //ScopedPtr<Nec, HeapDeleter> sp{ new(&h) Nec{465}, HeapDeleter<Nec>(h) };
    ScopedPtr sp{ new(&h) Nec{465}, HeapDeleter<Nec>(h) };
    std::cout << "in function f2 *sp = " << *sp << "\n\n";
}




void f3()
{
    auto vptr = std::malloc(sizeof(Nec));
    if (!vptr)
        return;
    auto necptr = new(vptr)Nec{ 555 };
    //ScopedPtr<Nec, FreeDeleter> sp{ necptr, FreeDeleter<Nec>{} };
    ScopedPtr sp{ necptr, FreeDeleter<Nec>{} };
    std::cout << "in function f5 *sp = " << *sp << "\n\n";
}


int main()
{
    f1();
    f2();
    f3();
}


