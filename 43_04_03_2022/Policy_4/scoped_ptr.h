#pragma once

#include <utility>

template <typename T>
struct DefaultDelete
{
    void operator()(T* p) const 
    {
        delete p;
    }
};


template <typename T, template <typename> typename DelPolicy = DefaultDelete>
class ScopedPtr {
public:
    explicit ScopedPtr(T* p = nullptr, const DelPolicy<T>& del_policy = DelPolicy<T>()) : m_p(p), m_del_policy(del_policy)
    {}

    ~ScopedPtr() 
    {
        m_del_policy(m_p);
    }

    T* release()
    { 
        return std::exchange(m_p, nullptr);
    }
    
    T* operator->() 
    {
        return m_p;
    }
    
    const T* operator->() const
    {
        return m_p;
    }
    
    T& operator*() 
    {
        return *m_p;
    }
    const T& operator*() const 
    {
        return *m_p;
    }
private:
    T* m_p;
    DelPolicy<T> m_del_policy;
    ScopedPtr(const ScopedPtr&) = delete;
    ScopedPtr& operator=(const ScopedPtr&) = delete;
};
