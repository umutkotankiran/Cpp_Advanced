/*

BU DERSTE TEMPLATELER ILE ILGILI TEKNIKLER GÖRÜLECEK

CRTP
----
Curiously Recurring Template Pattern
1995 te tanıtılmış bir teknik.
Burada taban sınıf olarak kullanılacak bir sınıf şablonu var. Bu sınıf şablonundan yapılacak kalıtımlar ile
aslında bu sınıf şablonunun template parametresini, oluşturulan türemiş sınıf olmasını sağlamak. 

Yani öyle bir katılım yapacağızki buradaki T türü aslında bizim sınıfımızdan kalıtım yoluyla elde edilecek.
Dolayısıyla bizim sınıf şablonundan elde edilecek sınıf kalıtım yoluyla elde edilecek sınıfın taban sınıfı olacak.

template <typename Derived>
class Base
{
public:
	void interface()
	{
		//..

		static_cast<Derived *>(this)->implementation();   // Der in implementation isimli func ı varsa biz herhangibir türemiş sınıf
								  // nesnesiyle bunu çağırırsak, türemiş sınıfın implementation funcı çağrılacak.

		//static_cast<Derived &>(*this).implementation(); // buda olabilirdi
	}

	static void static_interface()
	{
		Derived::static_implementation();
	}

	void implementation() // Türemiş sınıfta implementation olmazsa burası devreye girer
	{
		std::cout << "Base::implementation()\n";
	}

	static void static_implementation()
	{
		std::cout << "Base::static_implementation()\n";
	}
};

class Der : public Base<Der>
{
public:
	void implementation() // Bu yazılmasaydı, Basedeki implementation çalışacaktı.Onu blki ayrı yazarım
	{
		std::cout << "Der::implementation()\n";
	}

	static void static_implementation() // Bu tanımlanmasaydı taban sınıfın static implementationu çalışacaktı
	{
		std::cout << "Der::static_implementation()\n";
	}
};

int main()
{
	Der myder;

	myder.interface(); // Der::implementation() yazar.

	Der::static_interface(); // Der::static_implementation() // 

	-----------------------------------------------------------------------------------

	//Der deki static implementationu silersek
	Der::static_interface(); // base::static_implementation()

}

-------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------

KULLANICI HATASI
----------------
BİRDEN FAZLA SINIFI OLUŞTURURKEN YANLIŞLIKLA TEMPLATE ARGÜMANI OLARAK AYNI ARGÜMANI KULLANIRSAK 
BU ARTIK RUNTİME A YÖNELİK HATA OLACAK.AYNI SINIFIN 2 AYRI SINIF TÜRÜNDEN OLMA İHTİMALİ YOK
BUNU ENGELLEMEK İÇİN BASE SINIFA PRİVATE BİR CTOR YAZIP, TEMPLATE PARAMETRESİ OLACAK SINIFLARA
FRIENDLİK VERİLİR.

template <typename Derived>
class Base
{
public:
private:
	Base() { }        // Hata çözülmek için bu satırlar yazıldı. Artık Deleted functiona yapılan çağrıdan ötürü
	friend Derived;	  // Sentaks hatası
};

class Der : public Base<Der>
{
public:

};

class Nec : public Base<Der> // DİKKAT !!!!!!!! 
{			     //	Base<Nec> yerine yanlışlıkla Base<Der> yazılmış örnek böyle.
			     // UB OLUŞTURUYOR NORMALDE BURASI.BASE İÇİNE FRIENDLİK VE CTOR YAZARAK BUNUN 
			     // HATA OLMASINI SAĞLADIK.DELETED FUNC CALL
public:

};

KODU BİRAZ İNCELEYİNCE HATA GÖRÜLÜYOR !!!!!

-------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------

BURADAKI ASIL FIKIR, TABAN SINIFIN BIRŞEKILDE TÜREMIŞ SINIFI KULLANARAK TÜREMIŞ SINIFA BIRTAKIM IŞLEMLER EKLEMESI
Durumua göre kod miktarını düşürebilir yada başka avantajlar sağlayabilir.
Peki herhangibir sınıfa bu temaları eklemek istersek teker teker yazmadan CRTP ile ekleyeceğiz bazı özellikleri.

OBJECT COUNTING
---------------
template <typename T>
class Counter{
public:
	Counter() // Default ctor
	{
		++ms_count_created;
		++ms_count_alive;
	}
	
	Counter(const Counter &) // Copy ctor
	{
		++ms_count_created;
		++ms_count_alive;
	}

	~Counter()
	{
		--ms_count_alive;
	}

	static std::size_t how_many_created()
	{
		return ms_count_created;
	}
	
	static std::size_t how_many_alive()
	{
		return ms_count_alive;
	}
private:
	//BUNLAR BILDIRIM.TANIM DEĞIL.AŞAĞIDA TEMPLATE YAZARAK TANIMINI YAPTIK.C++17 İLE İNLİNE YAPABİLİRDİK AYRICA BURADA
	static std::size_t ms_count_created;
	static std::size_t ms_count_alive; // her farklı açılım için farklı static nesneler olacak.size_t yerine std::size_t kullanmak öneriliyor.
};


//TANIMLAR BURADA
template<typename T>
std::size_t Counter<T>::ms_count_alive{}; // 0 değerini aldı

template<typename T>
std::size_t Counter<T>::ms_count_created{};


class Neco : Counter<Neco> // private kalıtım.DİKKAT!! Bu class template değil. Template olsaydı Counter<Neco<T>> gerekecekti.
{
	//interface
public:
	using Counter<Neco>::how_many_created; // Taban sınıftaki isimler türemiş sınıfta visible hale geldi.
	
};

int main()
{
	Neco n1, n2, n3;

	{
		Neco n4, n5, n6;
	}

	std::cout << "how_many_created = "<<Neco::how_many_created() << "\n"; // Burada private kalıtımdan dolayı şuanda hata var.
									      // Bunu using bildirimi ile aşıyoruz.Modern C++ ta bu Ctor içinde yapabiliyoruz.Hatta adıda inherited Ctor.
	
	std::cout << "how_many_alive = "<<Neco::how_many_alive() << "\n";

}

Burada her bir nesne yaratıldığında, o nesnenin belirli açılımları için her seferinde taban sınıf
nesnesi create edilecek. VE sayısı artırılacak. Türemiş sınıftanda bu sayıyı öğrenebiliriz.

--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

MIXIN CLASS MANTIĞI
TÜREMİŞ SINIFLARA BELİRLİ BİR ÖZELLİĞİ KAZANDIRMAK İÇİN CRTP UYGULAYACAĞIZ

template <typename Derived>
class Writer{
public:
	void write(const char *str)const  // buraya dikkat !!!
	{	
		static_cast<const Derived *>(this)->write_impl(str);
	}	
};

class FileWriter : public Writer<FileWriter>
{
public:
	FileWriter(const char *pfilename) : mp_file(std::fopen(pfilename,"w"))
	{
		using namespace std::literals;
		if(!mp_file)
		{
			throw std::runtime_error{"file : "s + pfilename + " cannot created!"}; // dosya açılamazsa exception throw edecek
		}
	}
	
	friend class Writer<FileWriter>; // bu sayede write, write_impl call yapacak ve private olmasına rağmen çalışacak

private:
	void write_impl(const char *str)const
	{
		std::fprintf(mp_file,"%s\n",str);	
	}

	FILE *mp_file;
};


//BUNUDA YAZDIK EKSTRADAN
class ConsoleWriter : public Writer<ConsoleWriter>
{
public:
	friend class Writer<ConsoleWriter>;

private:
	void write_impl(const char *str)const
	{
		std::printf("%s\n",str);
	}
};

int main()
{
	FileWriter fw{"aaaa.txt"};
	
	ConsoleWriter cw;

	fw.write("Dolar kuru 10.67 oldu\n");
	cw.write("Dolar kuru 10.67 oldu\n");
}

--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

EŞİTLİK KIYASLAMASI 
-------------------

template <typename Der> // bu templatein amacı aşağıda her seferinde static cast yazmamak.
struct Equality{
	const Der& derived()const
	{
		return static_cast<const Der&>(*this); 
	}
};

template <typename Der>
bool operator==(const Equality<Der>&left, const Equality<Der>&right)
{
	return !(left.derived() < right.derived()) && !(right.derived() < left.derived());
}

template <typename Der>
bool operator!=(const Equality<Der>&left, const Equality<Der>&right)
{
	return !(left == right);
}

class Nec : public Equality<Nec>
{
public:
	Nec(int val = 0) : m_val{val} { }
	bool operator<(const Nec &other)const
	{
		return m_val < other.mval;
	}
private:
	int m_val;
}

class Person : public Equality<Person>
{
public:
	Person(std::string name) : m_name(std::move(name)) {}
	bool operator<(const Person &other)const
	{
		return m_name < other.m_name;
	}
private:
	std:string m_name;
}


int main()
{
	Nec x{198}, y{546};

	std::cout << std::boolalpha;
	std::cout << "x == y = " << (x == y) << "\n";
	std::cout << "x != y = " << (x != y) << "\n";

	Person p1{"Akif"},p2{"Akif"};
	std::cout << "p1 == p2 = " << (p1 == p2) << "\n";
	std::cout << "p1 != p2 = " << (p1 != p2) << "\n";

}

--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

FAKİR ADAMIN SPACESHIFT OPERATORU.STANDARTLAR BUNA TREEWAY COMPARASION DİYOR.
Yukarıdaki üzerinde değişiklik yaptık.

template <typename Der> // bu templatein amacı aşağıda her seferinde static cast yazmamak.
struct Comparison {
	const Der& derived()const
	{
		return static_cast<const Der&>(*this);
	}

	friend bool operator==(const Comparison<Der>& left, const Comparison<Der>& right)
	{
		return !(left.derived() < right.derived()) && !(right.derived() < left.derived());
	}

	friend bool operator!=(const Comparison<Der>& left, const Comparison<Der>& right)
	{
		return !(left == right);
	}

	friend bool operator>(const Comparison<Der>& left, const Comparison<Der>& right)
	{
		return (right.derived() < left.derived()); // bu durumda left right tan büyük olacak.
	}

	friend bool operator>=(const Comparison<Der>& left, const Comparison<Der>& right)
	{
		return !(left.derived() < right.derived()); // a küçük b değilse, a>= b dir. Mantık bu.
	}

	friend bool operator<=(const Comparison<Der>& left, const Comparison<Der>& right)
	{
		return !(right.derived() < left.derived()); // büyüktür ün değili, küçük eşittir oluyor.
	}
};


class Nec : public Comparison<Nec>
{
public:
	Nec(int val = 0) : m_val{ val } { }
	bool operator<(const Nec& other)const
	{
		return m_val < other.m_val;
	}
private:
	int m_val;
};

class Person : public Comparison<Person>
{
public:
	Person(std::string name) : m_name(std::move(name)) {}
	bool operator<(const Person& other)const
	{
		return m_name < other.m_name;
	}
private:
	std::string m_name;
};


int main()
{
	Nec x{ 198 }, y{ 546 };

	std::cout << std::boolalpha;
	std::cout << "x == y = " << (x == y) << "\n";
	std::cout << "x != y = " << (x != y) << "\n";

	Person p1{ "Akif" }, p2{ "Akif" };
	std::cout << "p1 == p2 = " << (p1 == p2) << "\n";
	std::cout << "p1 != p2 = " << (p1 != p2) << "\n";

}
--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

10 KEZ YAZDIRAN FUNC

template <typename Derived>
struct PrintNTimes{
public:
	void nprint(std::size_t n)const
	{
		while(n--)
		{
			static_cast<const Derived&>(*this).print();
		}
	}
};

class Person : public PrintNTimes<Person>
{
public:
	Person(std::string name, std::string surname) : m_name{std::move(name)},m_surname(std::move(surname)) { }
	void print()const
	{
		std::cout << m_name << " " << m_surname << '\n';
	}
private:
	std::string m_name;
	std::string m_surname;
};

int main()
{
	Person p{"Murat","Hepeyiler"};
	p.nprint(10);
}

--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

BURADA CONTAINER SINIFLARI İÇİN ORTAK KODU BİRYERE TAŞIMA FİKRİ VAR.FRONT BACK SİZE ... GİBİ FUNCLARI CRTP BASE E KAYDIRACAĞIZ.

template <typename Der>
class Container {
public:
	decltype(auto) front() // auto yapmadık çünkü autoda referanslık gidiyordu.
	{
		return *derived().begin();
	}

	decltype(auto) back() // auto yapmadık çünkü autoda referanslık gidiyordu.
	{
		return *std::prev(derived().end());
	}

	decltype(auto) size()const
	{
		return (size_t)(std::distance(derived().begin(), derived().end()));
	}

	decltype(auto) operator[](std::size_t idx)
	{
		return *std::next(derived().begin(), idx);
	}

private:
	Der& derived()
	{
		return static_cast<Der&>(*this);
	}
	const Der& derived()const
	{
		return static_cast<const Der&>(*this);
	}
};


template <typename T>
class Darray : public Container<Darray<T>> //DİKKAT !!! SINIFIN KENDİSİDE BU SEFER TEMPLATE.BU SEBEPLE DARRAY<T>
{
public:
	Darray(std::size_t n) : m_size{ n }, m_uptr{ std::make_unique<T[]>(n) } { }

	T* begin() {					// Containerda sınıfında bu funclara güvenerek yazılmış funclar var.
		return m_uptr.get();
	}

	const T* begin()const
	{
		return m_uptr.get();
	}

	T* end() {
		return m_uptr.get() + m_size;
	}

	const T* end() const
	{
		return m_uptr.get() + m_size;
	}

private:
	std::size_t m_size;
	std::unique_ptr<T[]> m_uptr;
};

int main()
{
	Darray<int> a{ 10 };

	for (size_t i{}; i < a.size(); ++i)
	{
		a[i] = i;
	}

	std::cout << a.front() << '\n';
	std::cout << a.back() << '\n';
}


MOLA


--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

POLYMORPHIC CHAINING AMAÇLI KULLANILIYOR.(FLUENT API)

class Myclass{
public:
	Myclass& func();
	Myclass& foo();
	Myclass& bar();
	Myclass& baz();
};

int main()
{
	Myclass m;

	m.func().bar().baz().foo(); // Fluent API deniyor bunlara.C++ta chaining daha yaygın
}


POLYMORPHIC CHAIN NE DEMEK?
Bazı senaryolar var. Mesela named parameter idiyom gibi senaryoların hepsi görülecek zaten.
Bazı işlemlerde işlemi yapabilmek için func return değerini biryerde saklamay gerekiyor onada çare oluyor.

Burada anlatılan ise şimdi nasıl yapıldığı.


#include <ostream>

class Printer {
public:
	Printer(std::ostream& os) : m_os{ os } {}

	template <typename T>
	Printer& print(const T& tval)
	{
		m_os << tval;
		return *this;
	}

	template <typename T>
	Printer& print_line(const T& tval)
	{
		m_os << tval << '\n';
		return *this;
	}
private:
	std::ostream& m_os;
};

#include <iostream>

class ConsolePrinter : public Printer {
public:
	ConsolePrinter() : Printer{ std::cout } {}

	ConsolePrinter& set_color(int)
	{
		return *this;
	}

};

//GEÇERLİ AMA BİR KALITIM İLİŞKİSİ OLDUĞUNDA ÇALIŞMIYOR

int main()
{
	ConsolePrinter().print("Kaveh Nematipour").set_color(661).print_line(9763.354);  //invalid. Set_color kısmı kırmızı
                                                   ----------------
}


--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------


POLYMORPHIC CHAIN YAPMAK İÇİN CRTP DEN FAYDALANIYORUZ.

template<typename ConcretePrinter>
class Printer{
public:
	Printer(std::ostream &os) : m_os{os} { }

	template<typename T>
	ConcretePrinter& print(const T& tval)
	{
		m_os << tval;
		return static_cast<ConcretePrinter&>(*this); //Klasik Crtp türdönüşümü.
	}

	template<typename T>
	ConcretePrinter& print_line(const T& tval)
	{
		m_os << tval << '\n';
		return static_cast<ConcretePrinter&>(*this);
	}

private:
	std::ostream &m_os;
};

class ConsolePrinter : public Printer<ConsolePrinter>
{
public:
	ConssolePrinter() : printer{std::cout} { }

	ConsolePrinter& setColor(int color)
	{
		//
		return *this;
	}
};

int main()
{
	ConsolePrinter().print("Kaveh Nematipour").setColor(654).print_line(9874); //GEÇERLİ
}

ÇIKTI
-----
Kaveh Nematipour9874

--------------------------------------------------------------------------------------------------------------------------

VARIANT RETURN TYPE - COVARIANCE A GİRİLDİ TEMEL C++
Burada o da kullanılabilirdi.

class A{};
class B : public A{};

class Base{

public:
	virtual A* func(); // ŞARTI REFERANS VEYA POINTER KULLANMAK !!!!!!!!!!!!!!!
};

class Der : public Base
{
public:
	B* foo()override;
	//B& foo()override;  Buda olur
};



--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------

STATIC POLYMORPHISM

class Animal{
public:
	virtual void cry() = 0; // Pure virtual function. Çok dandik bir uydurma. :D:D İmplementasyonu null anlamındaymış
	
};

class Cat : public Animal{
	void cry()override;
};

Bir sonraki derste derleyicinin virtual dispatch yerine kullanılacak kodu yazacağız.
Virtual dispatch olmadan virtual dispatch etkisi oluşturulacak.
Bunun için compile timeda crtp yi kullanacağız.


*/ 
