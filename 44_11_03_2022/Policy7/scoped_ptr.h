#include <utility>

template <typename T>
struct DefaultDelete {
    void operator()(T* p) const 
    {
        delete p;
    }
};

struct NoMove {
    NoMove() = default;
    NoMove(NoMove&&) = delete;
    NoMove(const NoMove&) = delete;
};


struct MoveForbidden {
    MoveForbidden() = default;
    MoveForbidden(MoveForbidden&&) = delete;
    MoveForbidden(const MoveForbidden&) = delete;
    MoveForbidden& operator=(MoveForbidden&&) = delete;
    MoveForbidden& operator=(const MoveForbidden&) = delete;
};

struct MoveAllowed {
};


template <typename T, typename DeletionPolicy = DefaultDelete<T>, typename MovePolicy = MoveForbidden>
class ScopedPtr
{
public:
    explicit ScopedPtr(T* p = nullptr, DeletionPolicy&& deletion_policy = DeletionPolicy()) : m_p(p), m_del_pol(std::move(deletion_policy))
    {}

    ScopedPtr(ScopedPtr&& other) : m_p(other.m_p), m_del_pol(std::move(other.m_del_pol)),
        m_move_pol(std::move(other.m_move_pol))
    {
        other.release();
    }

    ScopedPtr& operator=(ScopedPtr&& other)
    {
        if (this == &other)
            return *this;
        m_del_pol = std::move(other.m_del_pol);
        m_move_pol = std::move(other.m_move_pol);
        m_p = other.m_p;
        other.release();
        return *this;
    }

    ~ScopedPtr()
    {
        m_del_pol(m_p);
    }

    explicit operator bool()const
    {
        return m_p != nullptr;
    }

    void release()
    {
        m_p = nullptr;
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
    DeletionPolicy m_del_pol;
    MovePolicy m_move_pol;
    ScopedPtr(const ScopedPtr&) = delete;
    ScopedPtr& operator=(const ScopedPtr&) = delete;
};
