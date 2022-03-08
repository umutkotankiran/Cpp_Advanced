#pragma once

#include <cstddef>
#include <cassert>

class Heap {
public:
    Heap() = default;
    void* allocate(size_t s) 
    {
        assert(s <= ms_size);
        return m_buf;   // diziyi döndürdü
    }

    void deallocate(void* p) 
    {
        assert(p == m_buf);
    }
private:
    static constexpr size_t ms_size{ 1024 }; // compile time sabiti
    char m_buf[ms_size];    // bu bir dizi
    Heap(const Heap&) = delete;     // copy ve move a karşı kapalı. Copy memberları bildirince move memberlar deleted ediliyordu zaten.Ekstra yazmaya gerek yok.
    Heap& operator=(const Heap&) = delete;
};