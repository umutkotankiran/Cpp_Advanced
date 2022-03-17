#pragma once

#include <utility>

template <typename T>
struct DefaultDelete
{
    void operator()(T* p) const 
    {   if (p)
            delete p;
    }
};


template <typename P> 
class Releasable {
public:
    void release() 
    {
        static_cast<P*>(this)->m_p = nullptr; 
    }
};

template <typename P> 
class NoRelease {
};

template <typename T, typename DelPolicy = DefaultDelete<T>, template <typename> class ReleasePolicy = Releasable>
class ScopedPtr : private DelPolicy, public ReleasePolicy<ScopedPtr<T, DelPolicy, ReleasePolicy>>
{
public:
    explicit ScopedPtr(T* p = nullptr,
        DelPolicy&& deletion_policy = DelPolicy()
    ) : DelPolicy(std::move(deletion_policy)),
        m_p(p)
    {}

    ~ScopedPtr() 
    {
        DelPolicy::operator()(m_p);
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
    friend class ReleasePolicy<ScopedPtr>;
    T* m_p;
    ScopedPtr(const ScopedPtr&) = delete;
    ScopedPtr& operator=(const ScopedPtr&) = delete;
};
