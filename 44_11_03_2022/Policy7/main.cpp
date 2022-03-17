
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
struct HeapDeleter {
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
    ScopedPtr<int> sp1(new int(42));
    //ScopedPtr<int> sp2(std::move(p));  // invalid
    std::cout << *sp1 << '\n';
}

void f2()
{
    Heap h;
    ScopedPtr<int, HeapDeleter<int>> sp{ new(&h) int(42), HeapDeleter<int>(h) };
    std::cout << *sp << '\n';
}



void f3()
{
    ScopedPtr<int, DefaultDelete<int>, MoveAllowed> sp1{ new int(42) };
    std::cout << *sp1 << '\n';
    auto sp2(std::move(sp1));
    std::cout << (sp1 ? "dolu" : "bos") << '\n';
}


int main()
{
    f1();
    f2();
    f3();
}
