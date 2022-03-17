#pragma once

#include <cstddef>
#include <cassert>

class Heap {
public:
    Heap() = default;
    void* allocate(size_t s) 
    {
        assert(s <= ms_size);
        return m_buf;
    }

    void deallocate(void* p) 
    {
        assert(!p || p == m_buf);
    }
private:
    static constexpr size_t ms_size{ 1024 };
    char m_buf[ms_size];
    Heap(const Heap&) = delete;
    Heap& operator=(const Heap&) = delete;
};
