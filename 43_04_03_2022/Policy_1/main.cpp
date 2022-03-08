#include "necheap.h"
#include "nec.h"
#include "scoped_ptr.h"
#include <iostream>


struct FreeDeleter {
    template <typename T>
    void operator()(T* p) const
    {
        if (p) {
            p->~T();
            std::free(p);
        }
    }
};


struct DestroyOnly
{
    template <typename T>
    void operator()(T* p) const
    {
        p->~T();
    }
};


struct HeapDeleter{
    explicit HeapDeleter(Heap& heap) : m_heap(heap) {}
    template <typename T>
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

    ScopedPtr sp(new int{ 34 }); // C++17 den sonra CTAD ile böylede yazılır
    *sp = 8654;
    std::cout << "in function f1 *sp = " << *sp << "\n\n";
}

void f2()
{
    Heap h;
    //ScopedPtr<Nec, HeapDeleter>> sp{ new(&h) Nec{465}, HeapDeleter(h) };

    ScopedPtr sp{ new(&h) Nec{465}, HeapDeleter(h) }; // C++17 den sonra CTAD ile böylede yazılır
    std::cout << "in function f2 *sp = " << *sp << "\n\n";
    sp->set(998);
    std::cout << "in function f2 *sp = " << *sp << "\n\n";
}


void f3()
{
    //ScopedPtr<int, fdel_int> sp(new int{ 99 }, delete_int);

    ScopedPtr sp(new int{ 99 }, &delete_int);   // C++17 den sonra CTAD ile böylede yazılır.CTAD ta &delete_int yerine delete_int yazarsak function type elde edilir.
    std::cout << "in function f3 *sp = " << *sp << "\n\n";
}


void f4()
{
    //ScopedPtr<int, fdel_int> sp(new int(55), t_delete<int>);

    ScopedPtr sp(new int(55), &t_delete<int>);  // C++17 den sonra CTAD ile böylede yazılır
    std::cout << "in function f4 *sp = " << *sp << "\n\n";
}


void f5()
{
    auto vptr = std::malloc(sizeof(Nec));
    if (!vptr)
        return;
    auto necptr = new(vptr)Nec{ 555 };
    ScopedPtr<Nec, FreeDeleter> sp{ necptr, FreeDeleter{} };
    std::cout << "in function f5 *sp = " << *sp << "\n\n";
}


int main()
{
    f1();
    f2();
    f3();
    f4();
    f5();
}
