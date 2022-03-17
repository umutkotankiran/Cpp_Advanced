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
        if (p)
            p->~T();
        m_heap.deallocate(p);
    }
private:
    Heap& m_heap;
};

void f1()
{
    ScopedPtr<int, DefaultDelete<int>> sp(new int{ 34 });
    std::cout << "in function f1 *sp = " << *sp << "\n\n";
    sp.release();
}

void f2()
{
    Heap h;
    ScopedPtr<Nec, HeapDeleter<Nec>> sp{ new(&h) Nec{465}, HeapDeleter<Nec>(h) };
    std::cout << "in function f2 *sp = " << *sp << "\n\n";
    sp.release();
}



void f3()
{
    auto vptr = std::malloc(sizeof(Nec));
    if (!vptr)
        return;
    auto necptr = new(vptr)Nec{ 555 };
    ScopedPtr<Nec, FreeDeleter<Nec>, NoRelease> sp{ necptr, FreeDeleter<Nec>{}};
    // sp.release(); gecersiz
    std::cout << "in function f5 *sp = " << *sp << "\n\n";
}


int main()
{
    f1();
    f2();
    f3();
}
