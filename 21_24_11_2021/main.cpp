/*

Type erasure yeni örnek
Polymorphic diye bir tür yok. Kastedilen belirli türden varlıkların aynı türden gibi işleme sokulması. 
Aslında polimorphic behavior var.

Bunu polymorphic dispatch ile yapınca birsürü dezavantajı var.
Bunlar muhtemelken dinamik ömürlü olacak, pointer kulanacağız, birden fazla pointer aynı nesneyi gösterebilecek, 
bunların lifecycle yönetmi olacak,value type olmayacak... Value type çok önemli.

a = b; den sonra a ve b birbirinden bağımsız.Referans semantiği böyle değil
shared pointer böyle değil.

Kalıtımla biz yeni bir tür oluşturuyoruz.
Kalıtımla ortak özellikleri olan nesneleri aynıymış gibi ortak işleme sokabilmek amacımızdı.

----------------------------------------------------------------------------------------------------------------------------------

Geçen dersteki örneği bu sefer biraz farklı yazacağız.

1 - concept olarak kullanılan sınıf
2 - conceptten kalıtımla elde edilecek model sınıf

AÇIKLAMA ÖNEMLİ
---------------
- Pritable türünden bir nesne print edilebilecek.
- Printable türü type silme işi yapacak. Implementasyonda virtual func, pointer... var
  vector<Printable> pvec;
  pvec.emplace_back(12);
  pvec.emplace_back(1.2);
  pvec.emplace_back(Date{24,11,2021});  Bunların hepsi mümkün olacak.



#include <iostream>
#include <vector>
#include <memory>
#include <bitset>
#include "date.h"

template<typename T>
void print(const T& tval)
{
	std::cout << tval << '\n';
}

class Printable {
public:
	template<typename T>
	Printable(const T& x) : mptr{ std::make_unique<Model<T>>(x) } { } // Model conceptten kalıtımla elde edildiğine göre, uniqueptr<concept> bir modelide gösterebilir

	Printable(const Printable& other) : mptr{ other.mptr->clone() } { } // Copy Ctor olmazsa vectora de konulamaz

	Printable(Printable&&) = default;  // move ctor.

	Printable& operator=(Printable&&) = default; // move assing.

	Printable& operator=(const Printable& other)
	{
		return *this = Printable(other);  // İdiyom.Printable(other) copy ctor çağırıyor, *this = Printable(other) ifadesinin return değeri nesnenin kendisi
	}

	friend void print(const Printable& p)
	{
		p.mptr->print_impl();
	}

private:
	struct Concept
	{
		virtual ~Concept() = default; // Polymorphic nesne olduğundan destructoru virtual olacak.
		virtual void print_impl()const = 0; //
		virtual std::unique_ptr<Concept> clone()const = 0; // Kopyalama olması için clone idiyom kullanacağız
	};

	template <typename T>
	struct Model final : Concept  // Bundan başka türetme olmayacak.Final.Bİzim fiilen ortak özelliklerine dayanarak aynı türden gibi işleyecğimiz nesne.
	{
		Model(const T& x) : mx(x) {}

		std::unique_ptr<Concept> clone()const override
		{
			return std::make_unique<Model>(*this);
		}

		void print_impl()const override
		{
			print(mx); // Global funca çağrı.
		}

		T mx;
	};

	std::unique_ptr<Concept> mptr; // Printable pointer tutacak. Biz unique ptr yaptık.

};


int main()
{
	using namespace std;

	Printable p1(12);
	Printable p2(2.4);
	Printable p3(Date(24,11,2021));

	print(p1); // Bunu yazdığımızda friend func çağrıldı.İçindeki çağrı vir dispach oldu ve print impl
				// çağırdı. O da içeriden int türden nesne için global scopetaki printi çağırdı.
	print(p2);

	print(p3);
	
	-----------------------------------------------------------

	Printable bdate(Date(24,11,2021));
	string name {"kadir altindag"};

	std::vector<Printable> myvec;

	myvec.emplace_back(123);  // gönderdiğimiz argmanları perfect forward ediyor. Vectorde tutulan öğenin ctoru bu
	myvec.emplace_back(45.213);
	myvec.emplace_back(75.88);
	myvec.emplace_back(name);
	myvec.emplace_back(bdate);
	myvec.emplace_back(bitset<32>(54789));

	for(const auto &p : myvec)
		print(p);

}

=======================================================================================================================================================
=======================================================================================================================================================
=======================================================================================================================================================
=======================================================================================================================================================
=======================================================================================================================================================
=======================================================================================================================================================
=======================================================================================================================================================
=======================================================================================================================================================


VOCABULARY TYPES
----------------
C++17 ile geldi.

OPTIONAL
--------
Optional sınıfı türünden nesne bir değere sahipte olabilir olmayadabilir.
İkiside doğal bir durum. Bu yeni bir fikir değil. Functional programlama dillerinde uzun süredir var. 

Nerelerde İşe Yarayabilir
-------------------------
1 - Functiondan bir değer döndüreceğim. değer döndürüp döndürmediğimi anlatmak için optional kullanabilirim
2 - Bir func benden parametresine değer istiyor ama ben değer göndermek istemiyorum.burada kullanılaiblir.
3 - ÇOK ÖNEMLİ. Sınıfın bir veri elemanı olması. Veri elemanı değer tutacak ama sözkonusu nesnenin değeri olabilir yada olmayabilir.
4 - Nesnenin duruma göre oluşması çok önemli. Nesne oluşturulsun mu yoksa oluşturulmasın mı ?
	Yani istediğim zaman hayata getirebileceğim yada getirmeyeceğim nesne kavramı.

Bu fikir boosttan alınmış ama epey farklılıklar var aralarında.
İdeale yakın implemente edilmiş bir sınıf.

HEADER FILE OPTIONAL

---------------------------------------------------------------------------------------

template <typename T>
class optional{
public:
	//..funclar

private:
	bool _initialized;
	std::aligned_storage_t<sizeof(T), alignof(T)> _storage;
};
Storage elemanı her zaman kendi içinde. Heap kullanılmıyor.

int main()
{
	std::cout << sizeof(optional<int>) << '\n'; //8
	std::cout << sizeof(optional<double>) << '\n'; // 16
}

---------------------------------------------------------------------------------------

template <size_t n>
class nec
{
	unsigned char buffer[n];
};

template<size_t n>
using optype = std::optional<Nec<n>>;

int main()
{
	std::cout << sizeof(optype<128>{}); //129 byte
	std::cout << sizeof(optype<1024>{}); //1025 byte
}


---------------------------------------------------------------------------------------

CONSTRUCTORS
------------

int main()
{
	std::optional<int> x1; //Default Ctor. Değeri yok nesnenin

	optional<int> x2{2345}; 

	optional<string> x3{"mustafa"};

	optional x4{4.32}; //Ctaddan faydalanıyor
	optional x4{"alican"}; //Ctaddan faydalanıyor
	optional x4{"alican"s}; //Ctaddan faydalanıyor

}

---------------------------------------------------------------------------------------

IN_PLACE

Bir Ctora bu değerleri alıp doğrudan bellek alanında nesneyi hayata getirme anlamında ipucu veriyoruz.
Bu değer ayrı bir tür. Bir empty classs türünden oluşturulmuş global erişilebilir nesne.

#include <complex>

class point{
public:
	Point(double,double,double);
};

int main()
{
	optional<Point> x{1,5,7}; //Geçersiz

	optional<Point> x{in_place,1,5,7}; // GEÇERLİ. Bu şekilde oluşturabiliriz. Argümanları alıp bellek alanında point nesnemi oluştur demek.

	optional<complex<double>> x{in_place,34.4, 4.5}; //GEÇERLİ

	optional<vector<int>> x{in_place,{1,4,5,7,9}};

	-------------------------------------------------------------

	auto pred = [](int a, int ){ return abs(a) < abs(b);};

	optional<set<int,decltype(pred)>> x{in_place,{1,3,5,7,9}}; //C++20 ile geçerli
	
	optional<set<int,decltype(pred)>> x{in_place,{1,3,5,7,9},pred}; //C++17 ile de geçerli

}

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

MAKE_OPTIONAL

int main()
{
	auto x = make_optional(12); // türü optional<int> oldu
}

---------------------------------------------------------------------------------------

NOT : OPTIONAL LITERAL TYPE !!!

constexpr optional<int> foo(optional<int> x)
{
	return *x + 30; // Literal type olmasaydı sentaks hatası olurdu
}
	
int main()
{
	constexpr auto val = foo(123);

	std::cout << *val << "\n";
}


---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

BİR OPTİONAL İLE BAŞKA BİR OPTİONAL BAŞLATILABİLİR

int main()
{
	optional dx = 123.42;
	auto dy = dx;

	-------------------------------------------------

	Tür dönüşümü

	optional<double> x = 234; // Türler arası dönüşüm varsa, optional lar arası dönüşüm yapılır

	optional<int> y = x; // Geçerli

	-------------------------------------------------

	int a[10]{};

	optional x = a; // A dizi olmasına rağmen bir array decay olacak ve deduction guide ile x in türü optionalin int * açılımı olacak

	-------------------------------------------------
	
	optional x = "Mustafa"; // x optional<const char *> türü

	optional x = "Mustafa"s; // x optional<string> türü

	optional<string> x = "Mustafa"; // x optional<string> türü

}

---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------

NESNENİN DEĞERİNİ KULLANMAK

nullopt_t türü var.Bu bir empty class. nullopt_t türünden de bir nullopt sabiti var.
optional interfaceinde bş bir nesne  oluşturma yollarından biri olarak ve bazende
optional nesnesini boş hale getirmenin yolu olarak nullopt_t türünden nullopt nesnesi kullanılıyor.

int main()
{
	optional<int> x; 
	optional<int> x{nullopt_t}; // İkisi arasında bir fark yok.Nesne boş

	x = nullopt; // GEÇERLI. HATTA SINIF NESNESI ISE DESTRUCTOR ÇAĞRILIR BURADA.
}

---------------------------------------------------------------------------------------

NESNE BOŞMU DEĞİL Mİ

int main()
{
	optional<int> x(34);

	if(x) // operator bool çağrılır
	{
		//dolu ise girer	
	}
	else
	{
		//boşsa girer
	}
	
	--------------------------------------------------------

	if(x.has_value()) // 
	{
		// Dolu ise burası	
	}
	else
	{
		// Boş ise burası
	}

	--------------------------------------------------------

	std::cout << (x ? "has value" : "has no value");

	--------------------------------------------------------

	if(x == !nullopt)
	{
		// dolu ise girer
	}
	else
	{
		// boşsa girer
	}
}

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

NESNEYE ERİŞMEK İÇİN *, ->, value() KULLANABİLİRİZ.

* EXCEPTION THROW ETMİYOR.DEĞER YOKSA * UB OLUŞTURUR
Return değeri referans

int main()
{
	optional<int> x(55);

	*x = 56; // int türden değişkene 56 değerini atadık.Sarmalanan nesneye değer atandı yani
	
	x = 98; // Buda geçerli. Burada ise doğrudan optionalin atama operator funcı.

	-----------------------------------------------------------------------

	optional<int> x;

	*x = 23; // Değer tutmuyor başlangiçta

	std::cout << *x; // UB. Exception yok tabi

	-----------------------------------------------------------------------

	optional<Date> x{in_place,24,11,2021};

	std::cout << *x << '\n';

	std::cout << x->month_day() << '\n'; // * ve -> boş veya dolumu bilinmiyorsa kullanılmamalı

	-----------------------------------------------------------------------

	optional<Date> x{in_place,24,11,2021};
	
	if(x)
	{
		std::cout << *x << "\n"; // Tipik olarak bu idiyom kullanılabilir.
	}

}

--------------------------------------------------------------------------------

std::optional<std::string> foo();

int main()
{
	if(auto op = foo()) // Bu if with initializer değil. C++ ın ilk zamanlarından beri var.
	{					 // bu yazıldığından op doğrudan logical yorumlamaya tabi tutuluyor.
		op->append();
	}
}

BU şekilde if örneği için dynamic cast örneği yapıldı ben yazmadım.

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

VALUE 

Return değeri referans.Farkı ise exception throw ediyor.

optional<string> foo();

int main()
{
	optional<string> op {"murat hepeyiler"};

	try{
		std::cout << op.value() << "\n";
		op.value().pop_back();			// en sondaki r gitti.
		std::cout << *op<< "\n";
	}
	catch(const std::exception &ex)
	{
		std::cout << ex.what() << "\n";
	}
}

--------------------------------------------------------------------------------

int main()
{
	optional<string> op;

	try{
		std::cout << op.value() << "\n";
	}
	catch(const std::exception &ex)  //HATA YAKALANIR. bad_optional_access
	{
		std::cout << ex.what() << "\n";
	}
}

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

VALUE OR
Bizden bir argüman istiyor, return değeri optional doluysa optionalın değeri boşsa bizim gönderdiğimiz değer.

int main()
{
	optional<string> op1;
	optional<string> op2("Necati ergin");

	cout << op1.value_or("noname") << "\n"; // noname
	cout << op2.value_or("noname") << "\n"; // Necati Ergin
	
	--------------------------------------------------------------------

	string &r = op1.value_or("osman Eroglu"); //Sentaks hatası lvalue ref to rvalue expr.Tipik hata

}

----------------------------------------------------------------------------------
----------------------------------------------------------------------------------

EMPLACE

En önemli funclardan biri. Bu func ile istediğim zaman optionale bir değer yerleştirebilirim.

int main()
{
	optional<Date> op;

	op.emplace(12,10,1997); // Datein ctoru çağrıldı ve date nesnesi bununla başlatıldı.

	-----------------------------------------------------------------------------

	ZATEN DEĞER VARSA

	optional<Date> op(in_place,1,1,1987);

	op.emplace(2,4,2004); // İlk nesneyi destruct edip bunu construct eder.

}

---------------------------------------------------------------------------------

class Myclass{
public:
	Myclass(int x)
	{
		std::cout << "Myclass(int)\n";
	}
	
	~Myclass()
	{
		std::cout << "Destuctor\n";
	}

};

int main()
{
	std::cout << "main basladi\n";
	
	{
		optional<Myclass> op{in_place,12};
	} 
	
	std::cout << "main devam ediyor\n";
}

ÇIKTI
main basladi
Myclass(int)
Destuctor
main devam ediyor

---------------------------------------------------------------------------------

int main()
{
	std::cout << "main basladi\n";
	
	optional<Myclass> op{in_place,12};
	
	std::cout << (op ? "has value" : "has no value") << "\n";

	op.reset();

	std::cout << (op ? "has value" : "has no value") << "\n";

	std::cout << "main devam ediyor\n";

}

ÇIKTI
main basladi
Myclass(int)
has value
Destuctor
has no value
main devam ediyor

---------------------------------------------------------------------------------

int main()
{
	std::cout << "main basladi\n";

	optional<Myclass> op{in_place,12};

	std::cout << (op ? "has value" : "has no value") << "\n";

	op = nullopt;

	std::cout << (op ? "has value" : "has no value") << "\n";

	std::cout << "main devam ediyor\n";
}

ÇIKTI
main basladi
Myclass(int)
has value
Destuctor
has no value
main devam ediyor

---------------------------------------------------------------------------------

int main()
{
	std::cout << "main basladi\n";

	optional<Myclass> op{in_place,12};

	std::cout << (op ? "has value" : "has no value") << "\n";

	op = {}; // YİNE AYNI.DEFAULT CONSTRUCT EDİLMİŞ BİR GEÇİÇİ OPTIONAL NESNESI OLUŞUYOR VE ONU ATIYORUZ.
			 // 2 OPTIONAL NESNESİ BİRBİRİNE ATANABİLİYOIR
	 
	std::cout << (op ? "has value" : "has no value") << "\n";

	std::cout << "main devam ediyor\n";
}

ÇIKTI
main basladi
Myclass(int)
has value
Destuctor
has no value
main devam ediyor

---------------------------------------------------------------------
---------------------------------------------------------------------

İHTİMALLER


op.reset();
op = nullopt;
op = {};

MOLA

=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

OPTIONAL IN 2 SPECIALIZATIONUNA DIKKAT ETMEK LAZIM
BİRİNCİSİ POINTER AÇILIMI

void func(optional<int*> op) // YA BIR INT* DEĞERI TUTUYOR YADA INT * TUTMUYOR.
{
	if(*op && **op) // *OP DEREFERENCE DEĞIL, OP NIN DEĞERI VAR MI DEMEK. **OP ISE DEĞERINE ERIŞTIRIYOR.
	{
		std::cout << **op << "\n";
		delete *op;
	}
}

int main()
{
	func(new int{98});
}

---------------------------------------------------------------------
---------------------------------------------------------------------

DİĞERİ OPTIONAL BOOL AÇILIMI

int main()
{
	optional<bool> op;
	
	if(op && *op) // op doluysa ve değeri true ise 
	{
		// ...
	}
}

---------------------------------------------------------------------
---------------------------------------------------------------------

DEĞERİ OLMAYAN HERZAMAN DEĞERİ OLANDAN KÜÇÜK

int main()
{
	optional<int> x;
	optional<int> y(10);
	optional<int> z(-30);

	std::cout << boolalpha;

	std::cout << (x < z); // True
	std::cout << (x < y); // True
	// diğerlerinide yaz
}


int main()
{
	optional<bool> x;
	optional y =  true;
	optional z  = false;

	std::cout << boolalpha;

	std::cout << (x < y) << "\n"; // True
	std::cout << (z < y) << "\n"; // True. Değeri varsa değerleri karşılaştırılıyor.

	---------------------------------------------------------------------------
	
	optional<int> x;
	optional<int> y;
	optional<int> z(30);

	std::cout << boolalpha;

	std::cout << (x == y) << "\n"; // True
	std::cout << (z == 30) << "\n"; // True
	
	---------------------------------------------------------------------------

	optional<bool> x;
	optional y =  true;
	optional z  = false;

	std::cout << boolalpha;

	std::cout << (x == y) << "\n"; // false
	std::cout << (x == false) << "\n"; // false
	std::cout << (x == true) << "\n"; // false
	std::cout << (x == nullopt) << "\n"; // True !!!

}

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

NEDEN BİR FUNCIN RETURN DEĞERİ STD::OPTIONAL OLUR ?
OPTIONAL OLMADIĞINDA YAZDIĞIMIZ KODLARDA NE KULLANIYORDUK ? 
Bir funcın duruma göre değer döndürmemesi normal olabilir.
Func return değerinin olmaması durumunda boş optional döndürmesi birçok durumda en iyi seçenek.

1. Return Değeri adrestir ve değer döndürmeyeceği durumda nullptr döndürür.
   ör: fopen

2. Func return türünden kullanılmayacak, işin yapılamadığını veya döndürülecek değer olmadığını anlatan
   özel bir değer olması.Sentinel value olması durumu
   stringin arama sınıfı gibi.Return değeri index ,stringin size_type.Başarısız olduğunda ise string npos dödürüyor.

3. Func ın bool döndürmesi fakat out parametreyede sahip olması. Kötü durum bu.
   bool func(int x, T *p), nesne adresi gönderiyor bir değer yazabilir yazmayabilir.

4. Functionun pair döndürmesi.Set in insert funcı mesela.
   pair<set<int>::iterator, bool> türünden değer döndürüyor.
   pair<set<int>::iterator, bool> retval = myset.insert(45); // mesela. Artık auto var :D

 En kötü durumda bu. Ben değer döndürmüyorum diyor ama pair yine construct edilmiş bir nesne.Buda maliyet

 Bunların yanında optional çok iyi duruyor.

 ----------------------------------------------------------------------------
 ÖR:
 optional<string> get_middle_name();

 int main()
 {
	if(auto s = get_middle_name())
	{
		*s // kullanabiliriz.
	}
 }

 ----------------------------------------------------------------------------

 ÖR:
 optional<int> to_int(const std::string &s);

 int main()
 {
	string str;

	if(auto op = to_int(str))
	{
	
	}
 }

 */
