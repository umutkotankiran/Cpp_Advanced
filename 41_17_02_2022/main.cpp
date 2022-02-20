/*

IDIYOMLAR VE TEKNIKLERDEN DEVAM
-------------------------------
Friend bildirimleri ile ilgili bazı teknikler
Bir koda friendlik verdiğimizde, sınıfımızın private bölümüne erişim hakkı vermiş oluyoruz.
public private protected bölümlerinden yapılabilir.
Friendlik farklı nedenlerle veriliyor.

3 farklı friendlik verilebiliyordu. Temel C++ta gördük
1 - Sınıfa
2 - Sınıfın Member funcına
3 - Free/Global funca verilebilir.

Sınıfa friendlik verildiğinde bu incomplete type olabilir.

//class Neco; // Aşağıki bildirim için bu gerekmiyor.

class Myclass{
	
	friend class Neco;	// bunu yazdığımızda yukarıdaki forward decleration gerekmiyor
};


Diğeri ÖR

//struct Data;

struct Data foo(struct Data);  // yukarıdaki bildirim olmadanda struct keywördü ile birlikte kullanınca 
				// bu yine incomplete type olarak görev yapıyor. C ve C++ ta böyle

========================================================================================================================================
========================================================================================================================================
========================================================================================================================================
========================================================================================================================================


Biz her ne kadar uygun kullansakta friendlik data hidingi biraz bozuyor olabilir. Java C# tan gelenler pek beğenmiyor bunu.
Bir burada client kodlara friendlik vermiyoruz, kendi interfaceimizdeki sınıflara friendlik veriyoruz.
Nested classlara yada header fileda olan free/global funclara. Bunlar interfacein bir parçası.
Dolayısı ile data hiding bozuluyor demek çokta doğru değil.


ATTORNEY CLIENT IDIOM
---------------------
Buradaki Olay Belirli Öğelere Friendlik vermek
Doğrudan bunu yapamadığımız için yardımcı sınıf kullanacağız.
Yardımcı sınıfta asıl friendlik vermek istediğimiz sınıfın belirli private öğelerine erişecek.
Programlama dillerinde bu tip araçlar olmayınca belirli başlı teknikler geliştiriliyor.

class Nec
{
private:
	int ma{}, mb{};
	
	void pfoo1()
	{
		std::cout << "pfoo1 called!\n";
	}
	
	void pfoo2()
	{
		std::cout << "pfoo2 called!\n";
	}

	void pfoo3()
	{
		std::cout << "pfoo3 called!\n";
	}

	friend class Helper;
};

class Helper
{
public:
	static void pfoo1(Nec &rnec)
	{
		rnec.pfoo1();
	}

	static int get_ma(const Nec &rnec)
	{
		return rnec.ma;
	}
};

class Umut
{
public:
	void foo(Nec &r)
	{
		//Helper::pfoo1(r);  buda olur
		auto x = Helper::get_ma(r);	 // buda olur
	}
};

========================================================================================================================================
========================================================================================================================================
========================================================================================================================================


BU ÇOK ÖNEMLİ !!!!

HIDDEN FRIEND
-------------
Temel C++'ta çok kullanıldı ama varlık nedenini sorgulamadık.
Biz bir sınıfın içinde namespaceteki bir funca friendlik verebiliyoruz ama 
func tanımını inline olarak sınıfın içinde yapıyoruz.

ÖR:
class Myclass
{
public:
	friend void foo() // Friend bildirimi yapılan funclar, classı kapsayan namespacei içinde var olduğu kabul ediliyor.
	{				  
		///...
	}
};

int main()
{
	foo();	// Bu şekilde isim aranıp bulunamaz.Bunun istisnası ADL !!!
}


ÖR:
class Myclass
{
public:
	friend void foo(Myclass)
	{
		///...
	}
};

int main()
{
	Myclass m;
	foo(m);	// ADL ile bulundu.
}

Hidden friend denmesinin sebebi bu. Normalde namespace te visible değil ancak ADL ile kullanılabiliyor.
ADL ile bulunması için funcın en az bire paraemtresi olması gerekiyor.

Gizli olmasının nasıl bir faydası var?
Buradaki hidden friendin nedeni sınıfın private bölümüne erişmek olmak zorunda değil.
Birçok durumda bu tip friend olan funclar sınıfın friend bölümüne erişmeyedebilir.
Bu kısmı koruyabiliriz.Burada friendlik vermenin amacı bu ismi hide etmek, kullanılmasını engellemek.

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

SORU:

int main()
{
	std::vector x{10,20,30};

	std::cout << x; // Burada hata var.Vector için yazılmış bir inserter yok.
			// std namespace içinde adl ile uygun func aranıyor.
			// Derleyici bu funclara bakmak zorunda. Bu compile time ı uzatıyor.
			// Hemde karışık hataları ortaya çıkartıyor.Bu iyi birşey değil.
}

std::cout << "merhaba dunya\n"; // Burada da adl var
operator<<(std::cout, "merhaba dunya"); // std::operator... yazmamamızın sebebide ADL

ADL de ilgili namespacete arıyor ama sentaks hatası olması için namespaceteki tüm operator<< e 
bakması gerekiyor.

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

class Nec
{
public:
	Nec() = default;
	Nec(int val) : mval{val} { }
	friend Nec operator+(const Nec& lhs, const Nec& rhs) // hidden friend deniyor
	{
		return lhs.mval + rhs.mval;
	}
private:
	int mval{};
};

int main()
{
	Nec nx{5}, mn{45};
	auto nz = nx + ny;  // Bu func sadeec ADL ile çağrılır.
}

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

struct A{};
struct B{};
struct C{};
struct D{};
struct E{};

A operator+(const A&, const A&);
B operator+(const B&, const B&);
C operator+(const C&, const C&);
D operator+(const D&, const D&);
E operator+(const E&, const E&);

class Nec{ };

int main()
{
	Nec n1, n2;

	auto nx = n1 + n2; // Sentaks Hatası.Vector örneğindeki gibi gelecek hata.Tüm operator+ ları denedi compiler
						// bu operator+ lar aynı namespace te.

}

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------


HIDDEN FRIEND HALİNE GETİRECEĞİZ

#include <iostream>

struct A{
	friend A operator+(const A&, const A&);
};

struct B{
	friend B operator+(const B&, const B&);
};

struct C{
	friend C operator+(const C&, const C&);
};

struct D{
	friend D operator+(const D&, const D&);
};

struct E{
	friend E operator+(const E&, const E&);
};

class Nec{};

int main()
{
	Nec n1,n2;
	auto n3 = n1 + n2; // Sentaks hatası.Şimdi Sentaks hatası mesajı değişti.Sadece bir tane hata var.
}						// Diğer overloadlar görülmedi. Çünkü bunlar sadece ADL ile bulunabiliyor.

Burada compile time avantajıda var.Süre olarak avantaj. 
Namespace te görülür durumda olmadığından tek tek uyup uymadığına bakmıyor.

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

namespace N {
	struct A {
		friend void foo(A) { }
	};

	struct B {
		operator A();
	};

	void bar(A) { }
}

void func() {
	N::A a;
	bar(a);   // OK, bar is found via ADL
	foo(a);   // OK, foo is found via ADL

	N::B b;
	bar(b);   // OK, bar is found via ADL
	foo(b);   // NOT OK, foo cannot be found.SYNTAX ERROR
}

----------------------------------------------------------------------------------------------------------------------------------------

Foo funcı hidden friend idi. Namespace içerisine aldık.
Hata ortadan kalktı.

namespace N {
	struct A {

	};

	struct B {
		operator A();
	};
	void foo(A) { }
	void bar(A) { }
}


void func() {
	N::A a;
	bar(a);   // OK, bar is found via ADL
	foo(a);   // OK, foo is found via ADL

	N::B b;
	bar(b);   // OK, bar is found via ADL
	foo(b);   // OK, foo IS FOUND via ADL
}


----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

Hidden friendi standart library çok sık kullanıyor.
customization/specialization point oluşturma imkanı veriyor.

En çok kullanıldığı yapılar.
swap
size
begin
end
data gibi funclar.

Aşağıdakilerde var
rbegin
rend
end 
begin
crbegin
crend

Burada kullanılan idiyoma two-step ADL& fallback.

----------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class Darray{
public:
	Darray(T* p, std::size_t size) : m_pa{p}, m_size{size} { }
	~Darray()
	{
		delete[] m_pa;  // belki diziyi tutuyor diye array delete yaptık
	}

	T& operator[](std::size_t idx)
	{
		return m_pa[idx];
	}
	
	const T& operator[](std::size_t idx)const
	{
		return m_pa[idx];
	}

	std::size_t size()const
	{
		return m_size;
	}
	
	friend void swap(Darray &lhs, Darray &rhs)noexcept
	{
		using std::swap;		// 
		std::swap(lhs.pa, rhs.pa);
		std::swap(lhs.m_size, rhs.m_size);
	}

private:
	T* m_pa;
	std::size_t m_size;
	// ..
};

ADL NE ZAMAN DEVREYE GİRMEZ ?
1 - Nitelenmiş isimde(qualified name) devreye girmez. a::func(ax); gibi.
2 - func ismini parantez içine aldığımızda ADL devreye yine girmiyor (func)(ax);
3 - Yerel bir isim varsa isim arama sırasına takılabilir.
	
	int main()
	{
		extern int func;
		nec::A ax;
		func(ax); // Burada da ADL devreye girmez.
	}

========================================================================================================================================
========================================================================================================================================
========================================================================================================================================


COPY SWAP IDIYOM
----------------
Sınıfların atama operator funclarının yazılması için kullanılan bir teknik.
Modern C++ öncesinde vardı ama aradaki fark şu. Modern C++ tan önce special member
funclar 4 tane idi. Move memberlar yoktu. C++11 ile birlikte Move ctor ve move assignment
geldi. Daha önceki dönemde copy swap idiyomu o zaman copy assignmentta değildi operator assignmenettı
onu implemente etmenin tekniği olarak kullanılıyordu. Günümnüzde artık aynı zamanda move assingment içinde
kullanılıyor.

Copy assignment ve move assingment 2 ayrı func yazmak yerine tek func ile işimizi hallediyoruz.

1 - Faydalarından biri strong exception guarantee vermesi. Bunları Temel C++ ta detaylı işledik yazmıyorum.

2 - Bu Self assignment kontrolünü(operator= de kullanmıştık) yapmak zorunda da bırakmıyor.Bazı senaryolar var, 2 farklı referansı
	2 parametreli funca argüman olarak gönderiyoruz referans semantiği ile belki kendi içinde bir atama yapıyor. Burada sorun oluşlabilirdi
	mesela.

3 - Ortak kodu bir yerde toplamak.

COPY SWAP IDIYOM NEDIR =  BU ATAMA OPERATOR FUNCININ COPY CTOR KODUNU KULLANMASI DEMEK ASLINDA

----------------------------------------------------------------------------------------------------------------------------------------

ÖR:

template <typename T>
class Darray{
public:
	Darray(T* p, std::size_t size) : m_pa{p}, m_size{size} { }
	
	~Darray()
	{
		delete[] m_pa;  // diziyi tutuyor diye array delete yaptık
	}

	Darray(const Darray &other) : m_pa{new T[other.m_size]}, msize{other.m_size}
	{ 
		std::copy_n(other.m_pa,m_size,m_pa);
	}

	Darray(Darray&&other)noexcept : m_pa{std::exchange(other.m_pa, nullptr)}, 
	m_size{std::exchange(other.m_size,0)} {  }

	Darray& operator=(const Darray &other)
	{
		if(this == &other)	// Self assingment kontrolü
		{
			return *this;
		}
		
		delete m_pa;

		// Deep copy kodu burasida
		m_pa = new T[other.m_size];
		m_size = other.m_size;
		std::copy_n(other.m_pa,m_size, m_pa);
	}

	T& operator[](std::size_t idx)
	{
		return m_pa[idx];
	}

	const T& operator[](std::size_t idx)const
	{
		return m_pa[idx];
	}

	std::size_t size()const
	{
		return m_size;
	}

private:
	T* m_pa;
	std::size_t m_size;
	// ..
};

----------------------------------------------------------------------------------------------------------------------------------------

BURADA COPY SWAP İDİYOMUNA GELDİK

template <typename T>
class Darray{
public:
	Darray(T* p, std::size_t size) : m_pa{p}, m_size{size} { }

	~Darray()
	{
		delete[] m_pa;  // diziyi tutuyor diye array delete yaptık
	}

	Darray(const Darray &other) : m_pa{new T[other.m_size]}, msize{other.m_size}
	{
		std::copy_n(other.m_pa,m_size,m_pa);
	}

	Darray(Darray&&other)noexcept : m_pa{std::exchange(other.m_pa, nullptr)},
	m_size{std::exchange(other.m_size,0)} {  }

	//COPY SWAPI YAZIYORUZ
	friend void swap(Darray &lhs, Darray &rhs)noexcept  // swap funclarının noexcept olması gerekir.
	{
		using std::swap;  // Varsa custom bir swap func yoksa std namespace'ine fallback olsun

		swap(lhs.m_pa, rhs.m_pa); // Derleyici swap ismini görünce üstteki swapı görüyor ve bu ismi std namespace'inde
									// bir isim olduğu için arıyor.
		swap(lhs.m_size, rhs.m_size);
	}


	// ESKIDEN BÖYLE YAZILIYORDU
	//Darray& operator=(const Darray& other) 
	//{
	//	Darray copy{other}; // copy ctor exception throw ederse nesne üstünde bir değişiklik olmayacak.Böylece commit or rollback garanti olacak
	//						// çünkü nesneyi değiştirmedik.
	//	swap(*this,copy);
	//}


	//YENİSİ
	Darray& operator(Darray copy) // artık böyle yazılıyor.Bu funca gönderilen nesne sağ taraf değeriyse copy ctor, sağ taraf değeriyse move ctor.
	{
		swap(*this,copy); // zaten hata olursa yukarıda parametrede hata oluyor.
	}


	T& operator[](std::size_t idx)
	{
		return m_pa[idx];
	}

	const T& operator[](std::size_t idx)const
	{
		return m_pa[idx];
	}

	std::size_t size()const
	{
		return m_size;
	}

private:
	T* m_pa;
	std::size_t m_size;
	// ..
};




========================================================================================================================================
========================================================================================================================================
========================================================================================================================================
========================================================================================================================================


NON VIRTUAL INTERFACE - NVI
---------------------------
Sanal olmayan arayüz. Klasik yaklaşımda sınıfın sanal funclarını public yapıyoruz ve sanal funcları türemiş sınıflar override ediyor.
Nonvirtual interface idiyomu aslında template metod örüntüsününde özel bir amaçla kullanılması gibi düşünülebilir.
Burada sınıfın public bölümünde sanal olmayan funclar olacak, bu sanal olmayan funclar private bölümündeki sanal funcları çağıracak.
Bu bizim bildiğimiz template metod uygulaması
 
ÖR:
class Base{
public:

	void foo()
	{
		//...
		vfunc();
	}

private:
	virtual void vfunc();

};

Neden bir sanal funcların public değilde private yapıyoruz ?
1 - Sanal funclar bir customization point. yani bir sanal func vermekle bunu override ettiğimde bu kodu custom bir kod haline getiriyoruz.
	Benim funcım çalışacak bu çağrıldığında. Burada problem, customization aslında implementasyon, funcı sanal yaparak implementasyon ile 
	interface i birbirinden ayırmamış oluyoruz.

2 - Liskov substitution principle
	Taban sınıfın public funclarını çağıran kodla bir kontrata güveniyor bunlar.Taban sınıfın nesnesinin kullanılabileceği heryerde
	problemsiz olarak türemiş sınıfı türden nesnede kullanılabilir.Bütün kontrata uyacak. exceptionlar, preconditionlar, postconditionların
	sağlanması gibi.

a - İnterface nonvirtual yapılarak daha stabil değişmeyecek implementasyondan ayrılmış bir interface vermiş oluyoruz.

b - Artık türemiş sınıflara belirli şeyleri enforce etmiş oluyoruz.Mesela belirli işlemlerin yapılmasını sağyalabiliriz.
	Aynı zamanda ortak kodu bir yerde toplamak anlamınada geliyor bu.
	Aynı zamanda burada precondition kontrolüde yapabiliriz.
	Belki bazı koşullar sağlanmadığından exception throw edeceğiz.
	Yada bir debug sürümüdür burada dinamik assertion kullanabiliriz

Doğrudan sanal funcı koysaydık bu şansların hepsini kaybederdik

class Base{
public:

	void foo()
	{
		//precondition
		enforce();
		vfunc();
		//postcondition
	}

private:
	virtual void vfunc();
};


NVI'de implementasyon ile interface'i birbirinden ayırmış olduk. Daha stabil daha sağlam sanal funclardan oluşmayan interface kazandırdık.
	
Dikkat edilmesi gereken nokta sanal funcların private veya protected interfacete bulunması.
derived sınıf base classın private sınıfını veya protectedı override ediyor. Hatta türemiş sınıfın override ıda private
veya protected oluyor.

Burada protected neden seçilebilir ?
Türemiş sınıf taban sınıfın sanal funını override ettiğinde customization yapıyor ama bazı durumlarda taban sınıfın sanal funcının
kodunuda kullanıyor.

ÖR:
class Base{
public:

	void foo()
	{

	}

private:
	virtual void vfunc()
	{
		//
	}
};

class Der : public Base
{
	void vfunc()override
	{
		Base::vfunc();
		//our custom code
	}
};

----------------------------------------------------------------------------------------------------------------------------------------

#include <istream>
#include <ostream>
#include <cassert>

class Lock {
public:
	//...
	void acquire();
	void release();
};

class Data {
public:
	//...
	bool check_invariants()const;
};


class Base {
private:
	Lock m_lock;
	Data m_data;
public:
	void read(std::istream& iss)  //virtual değil
	{
		m_lock.acquire();
		assert(m_data.check_invariants()); // must be true

		do_read(iss);

		assert(m_data.check_invariants()); // must be true
		m_lock.release();
	}

	void write_to(std::ostream& o) // virtual değil
	{
		m_lock.acquire();
		do_write(o);
		m_lock.release();
	}

	virtual ~Base() {}
private:
	virtual void do_read(std::istream&) = 0;
	virtual void do_write(std::ostream&) = 0;
};


class XMLReaderWriter : public Base {
private:
	void do_read(std::istream&) override
	{
		// Read XML.
	}

	void do_write(std::ostream&) override
	{
		// Write XML.
	}
};


class JasonReaderWriter : public Base {
private:
	void do_read(std::istream&) override
	{
		// Read Jason.
	}

	void do_write(std::ostream&) override
	{
		// Write Jason.
	}
};


class TextReaderWriter : public Base {
private:
	void do_read(std::istream&) override
	{
		// Read Text
	}

	void do_write(std::ostream&) override
	{
		//write text
	}
};


*/
