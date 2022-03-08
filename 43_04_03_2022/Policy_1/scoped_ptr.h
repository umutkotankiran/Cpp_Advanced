#pragma once

#include <utility>

 template <typename T>  // sınıfıda template yapabiliriz sadece operator() funcınıda template yapabiliriz
struct DefaultDelete
{
    void operator()(T* p) const 
    {
        delete p;
    }
};


//DelPolicy yi kullanmanın 2 yolu var biri composition.C++20 ile EBO'ya bir alternatif var.[[no_unique_address]]
//sınıfın sizeofunu büyütmeden eleman olarak almak daha polüler hale gelebilir

//Diğer yol ise kalıtım. Eğer delpolicy sınıfı public kalıtım olsaydı interfacei kendi interfaceimize katmış oluyoruz.
//Ama sadece implementasyon kullanacaksa herzaman containment yoluyla compositiona bir alternatif private kalıtımı tercih ediyoruz.

template <typename T, typename DelPolicy = DefaultDelete<T>>  //Template tür parametresi default argüman almış.
class ScopedPtr {
public:
    explicit ScopedPtr(T* p = nullptr, const DelPolicy& del_policy = DelPolicy()) : m_p(p), m_del_policy(del_policy) // Varsayılan argüman kullanıldı.del_policy referans olmamalı
    {}                                                                                                               // çünkü dangling referans olur.Geçici nesnenin hayatı devam ediyor referans olarakta aktardık
                                                                                                                     // diyemiyoruz tabiki.
    
    ~ScopedPtr() 
    {
        m_del_policy(m_p);  //m_del_policy bir callable türü olmak zorunda.
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
    DelPolicy m_del_policy;
    ScopedPtr(const ScopedPtr&) = delete;
    ScopedPtr& operator=(const ScopedPtr&) = delete;
};
