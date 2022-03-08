/*


Konu Policy Based Designed.
Sınıf tasarımında burada amaç esnek ve genişletilebilir bir yapı oluşturmak.
Sınıf tasarlandıktan sonra bile daha sonra çıkan ihtiyaçları gereksinimleri karşılayabilecek
sınıflar oluşturmak.Avantajı ve dezavantajları var tabi.

Policy demekle sınıfın bir veya birden fazla davranışı için oluşturulan sözleşme.
Bir sözleşmeye dayanarak oluşturduğumuz sınıfları funcları veya callable ları
template argümanı olarak kullanıyoruz ve template kod içinde 
template argümanı olarak kullanılan  türün davranışları derleyicinin ürettiği kodda yer alıyor ve
böyle kod özelleştirilmiş oluyor. 10 lar ca farklı sınıf yerine tek bir sınıf şablonundan farklı sınıflar
oluşturabiliyoruz.

Bugün yazacağımız kodların anlaşılması için bazı noktaların anlaşılması gerekiyor.

1. VARSAYILAN TEMPLATE ARGÜMANI
-------------------------------
Function ın default argüman alması başka ama bir template in default template argümanı alması farklı şeyler.
bazı senaryolarda birlikte kullanılıyor.Benzeyen kurallarda var ama farklı olan kurallarda var.

Mesela functionlarda ve templatelerde varsayılan argüman alması için ikisinde de bir parametrede varsayılan
argüman varsa onun sağındaki tüm parametreler varsayılan argüman almalı.

Benzemeyen tarafıda aşağıdaki kodlarda var

void func(int x, int y = x); // Burası sentaks hatası
							 // Templatelerde ise bu yapılabiliyor

Template parametresi varsayılan argüman aldığında functionunda parametresi varsayılan argüman alıyor.
Hem func şablonunun template parametresi neden varsayılan argüman alıyor ?
Burada tema tempalte parammetresi varsayılan argüman aldığında functionunda varsayılan argüman alması
gerekiyor.

template <typename T, typename U = std::vector<T>> // Burada ise geçerli.Çünkü template
void func(T x, U y = U{}) // Temel C++ta da vardı.Burada const U& = U{} te U&& = U{} te olabilir
{
	std::cout << typeid(U).name(); // f(12) için = vector<int,allocator<int>> 
	std::cout << y.size(); // 0
}

int main()
{
	func(12); // 2. template parametresi vector<int>, 
				// func param değişkeni default init edilmiş vectorun int açılımı türünden nesne oluşturma ifadesi

	func(12,std::list<double>(23)); // bu sefer 2. parametre list<double> oldu
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

2. TEMPLATE TEMPLATE PARAMETER
------------------------------
type, nontype, template template parameterda alsa default arguman alabilir.

template<typename T>
class Nec{

};

//template<typename T, typename U = Nec<T>> // Bu olabilir
//class Myclass{
//
//};

template<typename T, typename U = Nec<T>>
class Myclass{
public:
	Myclass(T x, const U& = U());
};

int main()
{
	Myclass<int,double>x(12); // geçerli
	Myclass<int>x(12); // geçerli
	Myclass x(12); // CTAD var. Geçerli 
}

---------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
class Nec{

};

template<typename T, typename U>
class Myclass{
	Myclass(T x, const U& = U());
};

int main()
{
	Myclass x(12); // Invalid.CTAD devreye girmedi doğrudan
}

---------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATE TEMPLATE PARAMETREYE BAKIYORUZ

template<typename T>
class Nec{

};

template<template<typename> class C>  // class yerine typename de yazılabilir.
class Myclass{
	
};

int main()
{
	Myclass<int> x; // Geçersiz
	Myclass<Nec> x; // Geçerli
}

---------------------------------------------------------------------------------------------------------------------------------------------

template<typename T,typename U>
class Nec{

};

template<typename T>
class Nec2{

};

template<template<typename,typename> typename C> // Bu 2 template tür parametresi olan bir template
class Myclass{
	
};

int main()
{
	Myclass<Nec> x; // GEÇERLİ
	Myclass<Nec2> x; // GEÇERSİZ
}

---------------------------------------------------------------------------------------------------------------------------------------------

template<typename T,int>
class Nec{

};

template<template<typename,int> typename C> // Bir type birde nontype tür parametresi olan bir template istiyor.
class Myclass{

};

int main()
{
	Myclass<Nec> x; // GEÇERLİ
}

---------------------------------------------------------------------------------------------------------------------------------------------

template<template<typename,typename> typename C>
class Myclass{
	
};

int main()
{
	Myclass<std::vector> x; // Geçerli çünkü vector bir template.1 type 2. si allocator türünden parametre
}

---------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
class Nec{

};

template<template<typename> typename C>
class Myclass{
public:
	Myclass()
	{
		C<int>i;
		C<double>d;
		std::cout << typeid(C).name() << '\n';
		std::cout << typeid(i).name() << '\n';
		std::cout << typeid(d).name() << '\n';
		std::cout << typeid(Myclass).name() << '\n';
		std::cout << typeid(*this).name() << '\n';
	}

};

int main()
{
	Myclass<Nec> m;  // C nin türü yukarıda class Nec. 
			 // i nin türü Nec<int>
			 // d nin türü Nec<double>
			 // *this in türü Myclass<Nec>
			 // Myclass ın türü Myclass<Nec>
}

---------------------------------------------------------------------------------------------------------------------------------------------

En çok kullanılan yapılardan biri

template<typename T,typename U>
class Nec{

};

template<typename T, template<typename> typename C>
class Myclass{
public:
	Myclass()
	{
		std::cout << typeid(C<T>).name() << '\n';

	}
};

int main()
{
	Myclass<double,Nec>m;// C<T> nin türü Nec<double>
	Myclass<int,Nec>m;// C<T> nin türü Nec<int>
}

---------------------------------------------------------------------------------------------------------------------------------------------

DAHA FARKLI VARYASYONLARDA OLABILIR.

template<typename T,typename U>
class Nec{

};

template<typename T, typename U, template<typename,typename> typename C>
class Myclass{
public:
	Myclass()
	{
		std::cout << typeid(C).name() << '\n';
		std::cout << typeid(C<T,U>).name() << '\n';

	}
};

int main()
{
	Myclass<double, int, Nec>m; // Yukarıda C<T,U> demek,  Nec<double,int> demek
								 // C ise class Nec oluyor 
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

KULLANIM SENARYOLARI

template<typename T, typename A, template<typename,typename> class Con>
void func(const Con<T,A> &c)
{
	std::cout << typeid(T).name() << '\n';	// int
	std::cout << typeid(A).name() << '\n';  // Allocator<int>
	std::cout << typeid(c).name() << '\n';   // vector<int,allocator<int>>
	std::cout << typeid(Con).name() << '\n'; // vector
}

int main()
{
	vector<int> ivec;

	func(ivec);
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

BURASI İÇİNDE VARIADIC PARAMETER OLABİLİR
-----------------------------------------
Burası önemli. Yani variadic template için template parametre paketi type veya nontype içinde olabilir
ayrıca template template parameter içinde oluşturulabilir.

template<typename T>
class A{
	
};

template<typename T>
class B{
	
};

template<typename T>
class C{
	
};

//Geçerli burasıda ama her parametreyi tek tek yazmak gerekiyor.
//template<typename<typename> typename C1,typename<typename> typename C2>
//class Myclass{
//
//};

template<typename<typename> typename ...Ts>
class Myclass{
public:
	Myclass()
	{
		std::cout << typeid(Myclass).name() << '\n';
		std::cout << typeid(*this).name() << '\n';
		std::cout << typeid(std::tuple<Ts<int>...>).name() << '\n'; // tuple<A<int>,B<int>,C<int>>
	}
};

int main()
{
	Myclass<A> x1; // Myclass<class A>
	Myclass<A,B> x2; // Myclass<class A,class B>
	Myclass<A,B,C> x3; //  Myclass<class A,class B, class C>
}

---------------------------------------------------------------------------------------------------------------------------------------------

BENZER ÖRNEK

template<typename T>
class A{

};

template<typename T>
class B{

};

template<typename T>
class C{

};

template<typename T, typename<typename> typename ...Ts>
class Myclass{
public:
	Myclass()
	{
		std::cout << typeid(Myclass).name() << '\n';
		std::cout << typeid(*this).name() << '\n';
		std::cout << typeid(std::tuple<Ts<T>...>).name() << '\n'; // tuple<A<int>,B<int>,C<int>>
	}
};

int main()
{
	Myclass<int,A,B,C> x;
}

---------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATE TEMPLATE PARAMETER DEFAULT ARGUMENT

template <typename T>
class Myclass
{
public:
	
};

template<template<typename> class T = Myclass>  // burası önemli
class A
{ 

};

int main()
{
	A<Myclass> x;
	A x; // aynı
}

---------------------------------------------------------------------------------------------------------------------------------------------

NONTYPE PARAMETRE İÇİNDE GEÇERLİ

template <int x = 10, int y = 20>
class Myclass
{

};

int main()
{
	Myclass<5,9> x; // Myclass<5,9>
	Myclass<5> y;  //Myclass<5,20>
	Myclass <>y;	// Myclass<10,20>
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

CRTP DE VARIADIC TEMPLATE PARAMETER KULLANIMI
---------------------------------------------

CRTP HATIRLATMASI
-----------------

template<typename Der>
class A{
public:
	void func()
	{
		static_cast<Der*>(this)->f1();
	}
};

template<typename Der>
class B{
public:
	void foo()
	{
		static_cast<Der*>(this)->f2();
	}
};


class Myclass : public A<Myclass>, public B<Myclass>
{
public:
	void f1()
	{
		std::cout << "Myclass::f1()\n";
	}

	void f2()
	{
		std::cout << "Myclass::f2()\n";
	}
};

int main()
{
	Myclass mx;

	mx.foo();
	mx.func();
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

İSTEDİĞİMİZ KADAR BASE OLUŞTURABİLİR MİYİZ

template<typename Der>
class A{
public:
	
};

template<typename Der>
class B{
public:
	
};

template<typename Der>
class C{
public:
	
};

template<template<typename> typename ...Ts>
class Myclass : public Ts<Myclass<Ts...>>...
{
public:
	Myclass()
	{
		std::cout << typeid(std::tuple<Myclass<Ts...>>).name() << '\n';  // tuple<class Myclass<class A, class B, class C>>
		
		std::cout << typeid(std::tuple<Myclass<Ts...>>...>).name() << '\n';  // Açılımı bir alt satırda yazdım uzun biraz
		// tuple<class A<class Myclass<class A, class B, class C> >, class B<class Myclass<class A, class B, class C>>, class C<class Myclass<class A, class B,class C>> > 
	}
};

int main()
{
	Myclass<A, B, C> x;
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

template<typename Der>
class A{
public:
	void func()
	{
		static_cast<Der*>(this)->f1();
	}
};

template<typename Der>
class B{
public:
	void foo()
	{
		static_cast<Der*>(this)->f2();
	}
};

template<typename Der>
class C{
public:
	void bar()
	{
		static_cast<Der*>(this)->f3();
	}
};


template<template<typename> typename ...Skills>
class Myclass : public Skills<Myclass<Skills...>>...
{
public:
	Myclass()
	{
		std::cout << typeid(Myclass).name() << '\n'; 
		std::cout << typeid(std::tuple<Skills<Myclass<Skills...>>...>).name() << '\n'; 
	}

	void f1()
	{
		std::cout << "f1 cagrildi\n";
	}

	void f2()
	{
		std::cout << "f2 cagrildi\n";
	}

	void f3()
	{
		std::cout << "f3 cagrildi\n";
	}
};

int main()
{
	Myclass<A> x; // class Myclass<A>. interfaceine sadece func enjekte edildi
	// tuple<class A<class Myclass<class A>> > 
	x.func();

	Myclass<A,B> y; // class Myclass<class A,class B>. İnterface e func ve foo enjekte edildi.
	// tuple<class A<class Myclass<class A, class B> >, class B<class Myclass<class A, class B>> > 
	y.func();
	y.foo();
	

	Myclass<A,B,C> z; // class Myclass<class A,class B,class C>. İnterface e func,foo ve bar enjekte edildi.
							//1. taban sınıf								//2. taban sınıf										//3. taban sınıf		
	// tuple<class A<class Myclass<class A, class B, class C> >, class B<class Myclass<class A, class B, class C>>, class C<class Myclass<class A, class B,class C>> > 
	z.func();
	z.foo();
	z.bar();
}

MOLA

=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================

Bir pointer sınıfı yazıyoruz

//scoped_ptr.h
template<typename T>
class ScopedPtr
{
public:
	explicit ScopedPtr(T *p = nullptr) : m_p{p} { }

	~ScopedPtr()
	{
		delete m_p;
	}

	T* operator->()
	{
		return m_p;
	}
	
	const T* operator->()const
	{
		return m_p;
	}

	T* release()
	{
		return std::exchange(m_p,nullptr);
	}

	T& operator*()
	{
		return *m_p;
	}
	
	const T& operator*()const
	{
		return *m_p;
	}

private:
	T* m_p;
	ScopedPtr(const ScopedPtr &) = delete;
	ScopedPtr& operator=(const ScopedPtr &) = delete;
};


//nec.h
#pragma once
#include "iosfwd"
class Nec
{
public:
	Nec(int ival = 0);
	~Nec();
	void set(int ival)
	{
		m_val = ival;
	}
	
	int get()const
	{
		return m_val;
	}

	friend std::ostream& operator<<(std::ostream &os, const Nec&other);

private:
	int m_val;
};


//nec.cpp
#include "nec.h"
#include <iostream>

Nec::Nec(int val) : m_val {val}
{
	std::cout << m_val << " degerinde sınıf nesnesi olusturuldu\n"; 
}

Nec::~Nec()
{
	std::cout << m_val << " degerinde Nec nesnesi yok edildi\n"; 
}

std::ostream& operator<<(std::ostream&os, const Nec& nec)
{
	return os << "(" << nec.m_val << ")\n";
}

//main.cpp
#include "scoped_ptr.h"
#include "nec.h"

void func()
{
	ScopedPtr sptr {new Nec{23}};
}

int main()
{
	std::cout << "main basladi\n";
	func();
	std::cout << "main devam ediyor\n";
}

=============================================================================================================================================

GİTHUBA KOYULAN KODLAR ÇOK ÖNEMLİ

Policy_1
Policy_4
1:48

*/
