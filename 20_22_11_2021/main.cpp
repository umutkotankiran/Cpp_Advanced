/*

Bir kaç soruya cevap veriliyor.
crtp base den kalıtımla elde edilecek sınıfın template olması sözkonusu mu ? 
Genellikle evet.Crtp base tek olması zorunlu değil multiple inheritanceta olabilir.

template <typename Der>
struct MakeDouble{
	
	Der get_double()const
	{
		auto &d = static_cast<const Der&>(*this);

		return d + d; // operator+ nin türemişte olması gerekiyor 
	}
};

template <typename Der>
struct MakeTriple{
	
	Der get_triple()const
	{
		auto &d = static_cast<const Der&>(*this);

		return d + d + d;
	}
};


template <typename T>
class Value : public MakeDouble<Value<T>>, public MakeTriple<Value<T>>
{
public:
	Value(const T&val) : mval {val} {}
	
	Value operator+(const Value &other)const
	{
		return mval + other.mval;
	}

	void print()const
	{
		std::cout << mval << '\n';
	}

private:
	T mval;
};

#include<string>

int main()
{
	Value<int> x{12};

	auto val1 = x.get_double();
	auto val2 = x.get_triple();

	val1.print(); //24
	val2.print(); //36

	----------------------------------------------------------------

	Value<std::string> y{"burak"};
	auto val3 = y.get_double();
	auto val4 = y.get_triple();

	val3.print(); // burakburak
	val4.print(); // burakburakburak
}

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

NOT : VIRTUAL FUNCTIONLAR PRIVATE OLABILIR.ÇOKTA KULLANILIYOR.
NOT2: ACCESS CONTROL DİNAMİK TÜRE DEĞİL STATIC TÜRE GÖRE YAPILIR

class Base{
public:
	void foo()
	{
		// .. kodlar

		func();		// BURADA VIRTUAL DISPATCH MEKANIZMASI DEVREYE GİRER.
	}

private:
	virtual void func()
	{
		std::cout << "Base::func()\n";
	}
};

class Der : public Base{
public:
	void func()override // OVERRIDE EDILEBILIR.
	{
		std::cout << "Der::func()\n";
	}
};

void f(Base &base)
{
	base.func(); // BURADA SENTAKS HATASI.BASE İÇİNDE PRIVATE ÇÜNKÜ

	base.foo(); // BURASI İSE GEÇERLİ. SANALLIK MEKANIZMASI DEVREDE

}

int main()
{
	Der myder;

	f(myder); // DER IN FUNC I ÇAĞRILDI

	//DİKKAT!!!!
	myder.func(); //BURASIDA GEÇERLİ Der::func() çağrıldı
}

---------------------------------------------------------------------------------------

SIK KARIŞTIRILAN BAŞKA NOKTA

class Base{
public:
	virtual void func()
	{
		std::cout << "Base::func()\n";
	}
};

class Der : public Base{
private:
	void func()override
	{
		std::cout << "Der::func()\n";
	}
};

BURASIDA GEÇERLİ.

void f(Base &r)
{
	r.func();
}

int main()
{
	Der myder;
	f(myder); //GEÇERLI.TAMAMEN GEÇERLİ.ACCESS CONTROL DİNAMİK TÜRE DEĞİL STATIC TÜRE GÖRE YAPILIR

	myder.func(); // BURASI SENTAKS HATASI.BURADA STATIC TÜR YANİ . NIN SOL OPERANDI DER SINIFI TÜRÜNDEN
}					// DOLAYISIYLA KONTROL DER SINIFINA GÖRE YAPILIR.

---------------------------------------------------------------------------------------

YİNE AYNI DURUM "ACCESS CONTROL DİNAMİK TÜRE DEĞİL STATIC TÜRE BAĞLI YAPILIR"

class Base{
public:
	virtual void func(int x = 10)
	{
		std::cout << "Base::func(int) x " << x << "\n";
	}
};

class Der : public Base{
private:
	void func(int x = 50)override
	{
		std::cout << "Der::func()\n";
	}
};

BURASIDA GEÇERLİ.

void f(Base &r)
{
	r.func();
}

int main()
{
	Der myder;
	f(myder); // 10 YAZAR AMA DER::FUNC() ÇAĞRILIYOR.
}				// ARGÜMAN TABAN SINIFTAN GELDI.ARGÜMAN VIRTUAL OLANDAN DEĞIL.

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

Kalıtım çok dikkat edilmesi gereken bir ilişki çünkü bu çok kuvvetli bir bağımlılık oluşturuyor. 
Kalıtımı virtual dispatch ten ibaret düşünmüyoruz.Kalıtımında dezavantajlarıda var.Oluşan sınıf tamamen
taban sınıfla ilişkili.Kalıtıma başka alternatiflerde var. 

1 - CRTP = Yani static polymorphism kullanmak
2 - std::variant 
3 - Type Erasure idiom.En kuvvetli seçenek bu. Implementasyonuda baya karışık.Bu derste görülecek.
	Bir çok yerde var type erasure.

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

STATIC POLYMORHISM

BURASI RUNTIME POLYMORPHISM. BİR SONRAKI STATIC

class Pet
{
public:
	void make_sound()
	{
		std::cout << get_sound() << '\n';
	}

private:
	virtual std::string get_sound()const = 0;
};

class Cat : public Pet
{
public:
	virtual std::string get_sound()const override{
		return "miyav miyav miyav";
	}
};

class Dog : public Pet
{
public:
	virtual std::string get_sound()const override{
		return "hav hav hav";
	}
};

class Bird : public Pet
{
public:
	virtual std::string get_sound()const override{
		return "cik cik cik";
	}
};

void pet_game(Pet& pet)
{
	pet.make_sound();
}

int main()
{
	Cat mycat;
	Dog mydog;
	Bird mybird;

	pet_game(mycat);
	pet_game(mydog);
	pet_game(mybird); // Hepsinde virtual dispatch çalıştı
}

miyav miyav miyav
hav hav hav
cik cik cik

Burada işlemci maliyeti var. Dİnamik ömürlü nesnelerin heapte yaratılması başka nedenlerlede
programındaha yavaş olmasına sebep olabilir.

BUNU CRTP İLE YAPABİLİRİZ:

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

CRTP ILE YAPALIM

template <typename Der>
class Pet
{
public:
	void make_sound()
	{
		std::cout << derObject().get_sound();
	}
private:
	const Der& derObject()
	{
		return *static_cast<const Der*>(this); // biraz daha farklı yazdık, aslında öncekilerdekinin aynısı
	}
};

class Dog : public Pet<Dog>
{
public:
	std::string get_sound()const
	{
		return "hav hav hav";
	}
};

class Cat : public Pet<Cat>
{
public:
	std::string get_sound()const
	{
		return "miyav miyav miyav";
	}
};

class Bird : public Pet<Bird>
{
public:
	std::string get_sound()const
	{
		return "cik cik cik";
	}
};

template <typename T>  // Polymorphic yapıyı oluşturmak için bu template i yazdık.
void pet_game(Pet<T>&pet)
{
	pet.make_sound();
}

int main()
{
	Cat mycat;

	mycat.make_sound(); 

	---------------------------------------

	Cat mycat;
	Dog mydog;
	Bird mybird;

	mycat.make_sound();

	pet_game(mycat);
	pet_game(mydog);
	pet_game(mybird);

}

BENZER ŞEKİLDE ÇIKTILARI VERİR.
BURADA IŞLEMCI MALIYETI YOK VE NESNELERDE DINAMIK ÖMÜRLÜ OLMAK ZORUNDA DEĞIL.
HEAPTEN BELLEK ALANI ALLOCATE ETMEK ILE ILGILI UNSURLAR YOK.

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

SINGLETON İÇİN BİR CRTP BASE KULLANIYORUZ
Singleton kısaca bir sınıf türünden sadece bir nesne oluşturmamızı mümkün kılıyor.
2. bir nesne oluşturma olanağı vermiyor. Bİrsürü singleton implementasyonu var.


template <typename Derived>
class Singleton{
	static std::unique_ptr<Derived>		m_instance;
	static std::once_flag				m_once;   // bir funcın bir kere kulanılması için once_flag kullanılıyor.

protected:
	Singleton() {}

public:
	Singleton(const Singleton &) = delete;
	Singleton operator=(const Singleton&) = delete;
	static Derived& get_instance()
	{
		std::call_once(Singleton::m_once, [](){Singleton::m_instance.reset(new Derived)})

		return *m_instance;
	}
};

//Bunlar tanımlar.inline kullansaydık yazmaya gerekte kalmazdı.
template <typename Derived>
std::unique_ptr<Derived> Singleton<Derived>::m_instance;

template <typename Derived>
std::once_flag Singleton<Derived>::m_once;

class Nec : public Singleton<Nec>
{
public:
	Nec()
	{
		std::cout << "Nec default Ctor\n";
	}
	
	~Nec()
	{
		std::cout << "Nec Dtor\n";
	}

	void foo()
	{
		std::cout << "Nec::foo()\n";
	}

	void bar()
	{
		std::cout << "Nec:bar()\n";
	}
};

int main()
{
	Nec::get_instance().foo(); // Geçerli
	Nec::get_instance().bar(); // Geçerli.Singleton nesnesini kullanmış olacağız

	auto& snec = Nec::get_instance(); 

	snec.bar(); // Geçerli

	auto mynec = snec; // Burası hata verdi çünkü kopyalamaya çalıştık.Copy ctor delete edildiğinden
}

MOLA

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------

BİR SINIFIN KENDİSİNE DOKUNMADAN ONA EK ÖZELLİKLER KAZANDIRMA YÖNTEMİNE MIXIN DENIYOR

Böyle oluşturulan sınıflara crtp mixin deniyor. Kalıtımla mixin oluşturacağız
BU sefer ilave özellik kazandırmak istediğimiz sınıfı taban sınıfı olarak kullanacağız.
Yani CRTP nin tam tersi.CRTP de taban sınıf türemiş sınıfı kullanıyordu
Biz ise türemiş sınıfın taban sınıfa özellik kazandırmasını sağlayacağız.

BUrada da türemiş olan SkillA sınıfı, taban sınıfının interfaceine güvenerek kodu
oluşturuyor. Daha öncede tam tersini görmüştük 

template <typename Base>
class SkillA : public Base
{
public:
	void afunc()
	{
		std::cout << "SkillA is running\n";
		Base::foo();
		Base::bar();
		std::cout << "---------------------------------------\n";
	}
};

template <typename Base>
class SkillB : public Base
{
public:
	void bfunc()
	{
		std::cout << "SkillB is running\n";
		Base::baz();
		Base::foo();
		std::cout << "---------------------------------------\n";
	}
};

template <typename Base>
class SkillC : public Base
{
public:
	void bfunc()
	{
		std::cout << "SkillC is running\n";
		Base::foo();
		Base::foo();
		Base::foo();
		std::cout << "---------------------------------------\n";
	}
};

//OLUŞAN SINIFLAR MYCLASSI TABAN SINIFI OLARAK KULLANACAKLAR
class Myclass{
public:
	void foo()
	{
		std::cout << "Myclass::foo()\n";
	}
	
	void bar()
	{
		std::cout << "Myclass::bar()\n";
	}
	
	void baz()
	{
		std::cout << "Myclass::baz()\n";
	}
};

using Neco = SkillA<Myclass>;

int main()
{
	SkillA<Myclass>{}.afunc();
	
	SkillB<Myclass>{}.bfunc();

	-------------------------------------------------------------------------------------

	SkillA<SkillB<SkillC<Myclass>>> x; // 3 yetenek te eklendi

	x.afunc();
	x.bfunc();
	x.cfunc(); // Hepsi çalışıyor.Hepsi birbirinden kalıtımla elde edilmiş yukarıda kilit nokta bu
	
}

SkillA<Myclass> demek ile bır sınıf oluştur ve tür argümanı olarak verdiğim sınıfı taban sınıf olarak kullandı
Böyle istediğim kadar işlevsellik ekleyebilirim.

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------

CRTP İLE MIXIN OLUŞTURMAYI VE YUKARIDAKİ TEKNİKLE MIXIN OLUŞTURMAYI AYNI ÖRNEKTE YAZACAĞIZ


template <typename Printable>
struct RepeatPrint{
public:
	void repeat(unsigned n)
	{
		while(n--)
		{
			static_cast<const Printable&>(*this).print();
		}
	}
};

class Name : public RepearPrint<Name>
{
public:
	Name(std::string name, std::string surname) : m_name{std::move(name)},m_surname(std::move(surname)) { }
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
	Name Myname{"Necati","Ergin"};
	
	myname.repeat(10);
}


------------------------------------------------------------------------------------------

class Name
{
public:
	Name(std::string name, std::string surname) : m_name{std::move(name)},m_surname(std::move(surname)) { }
	void print()const
	{
		std::cout << m_name << " " << m_surname << '\n';
	}
private:
	std::string m_name;
	std::string m_surname;
};

template <typename Printable>
struct RepeatPrint : Printable
{
	explicit RepeatPrint(const Printable &printable) : Printable(printable) { } // Name gelirse const Name& olacak parametre. 
																				// O da taban sınıfın ctorunu çağırıyor.

	using Printable::Printable; // Inherited Ctor. Taban sınıf ctoru türemişte görülür halde artık.Temel C++

	void repeat(unsigned n)const
	{
		while(n--)
		{
			this->print(); // Printable::print(); te olur.			
		}					// HOCAYA SORDUM !!! Template kodda isim arama isim nitelenmedikçe taban sınıfta aranmıyor
	}
};

using RepeatPritableName = RepeatPrint<Name>;

int main()
{
	RepeatPrint<Name> myname{ Name{"Necati","Ergin"} };
	myname.repeat(10);

	RepeatPritableName x{"Umut","Kotankiran"};
	x.repeat(10);
}

Necati Ergin
Necati Ergin
Necati Ergin
Necati Ergin
..
.. 10 kere yazar ikisinde de

1:39
Burada bir örnek daha verildi onu yazmayacağım o bu örneğin üzerine bir template 
yazılarak yapıldı 

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------

TYPE ERASURE IDIOM
------------------
Farklı dillerde farklı anlamda kullanılıyor. BUrada ise farklı farklı türler var diyelim fakat biz
bu türlerin ortak özelliklerine dayanarak aynı türdenmiş gibi kullanabileceğim. Amaç bunu
sağlamak.

type erasure ile type erasure idiyom farklı şeyler.

Kalıtımla bu yapılabilir ama taban sınıf gerekiyor ve bunun sanal funclarını override etmek
gerekiyor. Bizim istediğimiz kalıtım olmadan bunu yapabilmek. Buna type erasure idiyom deniyor.

type erasure C de de var. void pointerlar ile yapılıyor.C nin standart kütüphanesindeki void ptr
funclarını düşünelim. Mesela qsort. stdlib.h de yer alıyor.

qsort türden bağımsız bir diziyi sıralıyor. 

void qsort(void *vpa, size_t size, size_t sz,int(*f)(const void *, const void *));

int mycmp(const void *vp1, const void *vp2)
{
	//*(const int *)vp1 - *(const int *)vp2; // Burada büyük değerlerde taşma olabilir.

	if(*(const int *)vp1 > *(const int *)vp2)
		return 1;
	if(*(const int *)vp1 < *(const int *)vp2)
		return -1;

	return 0;
}

int main()
{
	constexpr int size = 10'000;

	int a[size];

	generate_n(std::begin(a),size,Irand{0,10000});

	print(begin(a),end(a));

	qsort(a,size,sizeof(*a),&mycmp);
}

Burası C deki type erasure

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------

BURASI EN KARIŞIK YER !!!!!

Bizim bahsettiğimiz C++ taki type erasure idiyom. Yukarıdaki ile alakalı yok.
Aslında kalıtımda bir type erasure.

Amaç: Ortak bir interface e sahip türleri aynı türdenmiş gibi işleyebilmek için bir yapı oluşturmak.
Fakat bu yapıda sınıfları doğrudan kalıtım yoluyla doğrudan bir sınıftan oluşturmuyoruz.
Burada amaç aynı interface e güvenerek bu sınıfı oluşturmak

-----------------------------------------------------------------------------------------

Burada kullanılan bazı terimler var
Concept 
Model

-----------------------------------------------------------------------------------------

BURADAN BAŞLADIK 

class AnimalConcept // Bu concept olarak kullandığım sınıf model olarak kullanılacak sınıfa bir interface sağlıyor.
{
public:
	virtual ~AnimalConcept() = default;

	virtual void cry() = 0; // Sanal function ama buradaki virtual dispatch interfacete değil. Buradaki v.dispatch implementasyonda kalacak
};							// Runtime polymorphism clientlar açısından olmayacak


template <typename ErasedType>
class AnimalModel : public AnimalConcept{
public:
	
	//Hikaye Burada satırda döndü
	AnimalModel(const ErasedType&object) : object_{object} {  }  	//Func Parametreye dikkat !!!
	
	
	//Cry functionunu overload ediyor
	void cry()override
	{
		object_.cry(); // Türü silinecek türün, cry functionu olduğunda bu nesne için cry func çağrıldığında
	}					// türü silinmiş nesnenin cry functionu çağrılacak.
	
private:
	ErasedType object_;
};

YUKARIYI ANLADIKTAN SONRA AŞAĞIDAN DEVAM

--------------------------------------------------------------------------------------------------------

KODUN DEVAMI

// Bir sınıf içine alındı 1. ADIM
class Animal {
public:

	//Bu Ctor bir template, herhangibir sınıf türden nesneyi kabul edecek.
	//Veri elemanı olan unique ptr yi dinamik ömürlü bir nesne ile init edecek.Clientların haberi yok tabi
	template <typename ErasedType> // 4. ADIM
	Animal(ErasedType&& x) : uptr(std::make_unique<AnimalModel<ErasedType>>(std::forward<ErasedType>(x))) {}

	void cry() // 3. ADIM
	{
		uptr->cry(); // uptr model türünden nesneyi gösteriyorsa çağrılan func vir dispatch ile türemiş sınıfın override olacak
	}				 // Buda eleman olarak aldığı template parametresi olan sınıf türden nesnenin cry funcını çağıracak.

private: // Yukarıdaki nested type yaptık ve private oldular.Clientlerın bundan haberi yok.

	class AnimalConcept // Bu concept olarak kullandığım sınıf model olarak kullanılacak sınıfa bir interface sağlıyor.
	{
	public:
		virtual ~AnimalConcept() = default;

		virtual void cry() = 0; // Sanal function ama buradaki virtual dispatch interfacete değil. Buradaki v.dispatch implementasyonda kalacak
	};						    // Runtime polymorphism clientlar açısından olmayacak


	template <typename ErasedType>
	class AnimalModel : public AnimalConcept {
	public:

		//Hikaye Burada satırda döndü
		AnimalModel(const ErasedType& object) : object_{ object } {  }  //Func Parametreye dikkat !!!


		//Cry functionunu overload ediyor
		void cry()override
		{
			object_.cry(); // Türü silinecek türün, cry functionu olduğunda bu nesne için cry func çağrıldığında
		}					// türü silinmiş nesnenin cry functionu çağrılacak.

	private:
		ErasedType object_;
	};


	//Animal sınıfının pritave bölümüne bir unique ptr koyacağız.Shared ptr de raw ptrda olur ama unique ptr burada ideal
	//Yani animalconcept türünden bir dinamik ömürlü nesneyi gösterecek.Animalconceptten kalıtımla elde edilmiş
	// sınıf nesnelerinide gösterebilir unique ptr;

	std::unique_ptr<AnimalConcept> uptr;  // 2. ADIM

};

//BURADA KODU TEST EDİYORUZ.

class Cat{
public:
	void cry()
	{
		std::cout << "miyav miyav miyav\n";
	}
};

class Dog{
public:
	void cry()
	{
		std::cout << "Hav Hav Hav\n";
	}
};

class Bird{
public:
	void cry()
	{
		std::cout << "cik cik cik\n";
	}
};

int main()
{
	Animal a{ Bird{} };
	Animal b{ Dog{} }; // Herhangibir kalıtım ilişkisi yok gibi duruyor ama animal bir dog ile init edildi mesela
	Animal c{ Cat{} }; 
	
	a.cry();
	b.cry();
	c.cry(); // Bunları containerda da tutabilirim.
}

ÇIKTI
-----
cik cik cik
Hav Hav Hav
miyav miyav miyav

Runtime polymorphism e kalıtım dışında daha sağlıklı bir alternatif yaratıyor. Kalıtım ile elde etseydik
kalıtımla gelen bağımlılık sözkonusu olacaktı. 

BİRAZ DAHA KOD YAZILACAK. ÇÜNKÜ SINIFIN VERİ ELEMANI BİR UNİQUE PTR VE COPY CTOR DELETE EDİLMİŞ.
İSTEDİĞİMİZ BUNLARI AYNI TÜRDEN OLARAK ELE ALIP BİR CONTAİNERDA TUTMAK

--------------------------------------------------------------------------------------------------------

DEVAMI

ANİMAL SINIF TÜRÜNDEN NESNELERİN UNIQUE PTR ELEMANLARI OLDUĞUNDAN ANIMAL SINIFI TURUNDEN NESNEYİ
BAŞKA ANİMAL TÜRÜNDEN HATAYA GETİRMEK İSTESEYDİM SENTAKS HATASI OLURDU. BUNU YAMNAK İÇİN CLONE FUNC YAZACAĞIZ

5. ADIMI TAKİP ET

// Bir sınıf içine alındı 1. ADIM
class Animal {
public:

	void cry() // 3. ADIM
	{
		uptr->cry(); // uptr model türünden nesneyi gösteriyorsa çağrılan func vir dispatch ile türemiş sınıfın override olacak
	}				 // Buda eleman olarak aldığı template parametresi olan sınıf türden nesnenin cry funcını çağıracak.

	//Bu Ctor bir template, herhangibir sınıf türden nesneyi kabul edecek.
	//Veri elemanı olan unique ptr yi dinamik ömürlü bir nesne ile init edecek.Clientların haberi yok tabi
	template <typename ErasedType> // 4. ADIM
	Animal(ErasedType&& x) : uptr(std::make_unique<AnimalModel<ErasedType>>(std::forward<ErasedType>(x))) {}

	//BU Copy ctor yazılmazsa animal nesneleri birbirine kopyalanamaz çünkü unique ptr elemanı var.
	Animal(const Animal &other) : uptr(other.uptr->copy()) { } // 7. ADIM

	
	Animal& operator=(const Animal& other)
	{
		return *this = Animal(other);
	}

	Animal(Animal&&)noexcept = default;
	Animal& operator=(Animal&&)noexcept = default;

private: // Yukarıdaki nested type yaptık ve private oldular.Clientlerın bundan haberi yok.

	class AnimalConcept // Bu concept olarak kullandığım sınıf model olarak kullanılacak sınıfa bir interface sağlıyor.
	{
	public:
		virtual ~AnimalConcept() = default;

		virtual void cry() = 0; // Sanal function ama buradaki virtual dispatch interfacete değil. Buradaki v.dispatch implementasyonda kalacak
							    // Runtime polymorphism clientlar açısından olmayacak
		
		//copy isimli bir func koyduk, bu clone yapacak.Bu durumda animalmodel bu funcı override etmeli
		virtual std::unique_ptr<AnimalConcept> copy()const = 0; // 5. ADIM

	};	

	template <typename ErasedType>
	class AnimalModel : public AnimalConcept {
	public:

		//Hikaye Burada satırda döndü
		AnimalModel(const ErasedType& object) : object_{ object } {  }  //Func Parametreye dikkat !!!

		//Cry functionunu overload ediyor
		void cry()override
		{
			object_.cry(); // Türü silinecek türün, cry functionu olduğunda bu nesne için cry func çağrıldığında
		}					// türü silinmiş nesnenin cry functionu çağrılacak.

		std::unique_ptr<AnimalConcept> copy()const override // 6. ADIM
		{
			return std::make_unique<AnimalModel>(*this);
		}

	private:
		ErasedType object_;
	};


	//Animal sınıfının pritave bölümüne bir unique ptr koyacağız.Shared ptr de raw ptrda olur ama unique ptr burada ideal
	//Yani animalconcept türünden bir dinamik ömürlü nesneyi gösterecek.Animalconceptten kalıtımla elde edilmiş
	//Sınıf nesnelerinide gösterebilir unique ptr;

	std::unique_ptr<AnimalConcept> uptr;  // 2. ADIM

};

//BURADA KODU TEST EDİYORUZ.

class Cat{
public:
	void cry()
	{
		std::cout << "miyav miyav miyav\n";
	}
};

class Dog{
public:
	void cry()
	{
		std::cout << "Hav Hav Hav\n";
	}
};

class Bird{
public:
	void cry()
	{
		std::cout << "cik cik cik\n";
	}
};

int main()
{
	std::vector<Animal> avec;

	avec.emplace_back(Dog());
	avec.emplace_back(Cat());
	avec.emplace_back(Bird());

	for(auto &a : avec)
	{
		a.cry();
	}
}
	
KODDA TANIMSIZ DAVRANIŞ OLABİLİR


*/
