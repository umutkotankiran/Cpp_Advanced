/*

cooked ile uncooked arasındaki farka tekrar değindi hoca.

UNCOOKED
void operator""_x(const char *)
{
	std::puts(p); // yada yazı for ile tek tek dolaşılır
}

int main()
{
	456_x; // operator""_x("456"); derleyici bu şekilde çağrı yaptı
	"mehmet"_x; // BU SENTAKS HATASI UNCOOKED TA
}


COOKED
void operator""_x(const char *p, size_t)
{
	
}

int main()
{
	4567_x;// cooked uda bu şekilde çağıramayız.
	
	"mustafa"_x; // bu şekilde çağırabiliriz. 1. argüman yazı adresi 2. si ise uzunluk burada 7
}

-------------------------------------------------------------------------------------------------

BITWISE YAZDIRMA KODU

constexpr unsigned int operator"" _b(const char* p)
{
	unsigned uval = 0;

	while (*p) {
		char digit = *p;
		if (digit != '1' && digit != '0') {
			throw std::runtime_error("0 ya da 1 olmayan karakter\n");
		}

		uval = uval * 2 + (digit - '0');
		++p;
	}

	return uval;
}

int main()
{
	//constexpr auto val = 0b01001'1100; // bu seperator bizim görsel ayırımımız
	//constexpr auto val = 0b01'00'11'10'0; // böylede yazabilirdim

	//bunu uncooked olarak yazalım
	try{
		constexpr auto val = 010101101_b
		cout << "val = " <<val << "\n";
	}
	catch(const std::exception&ex)
	{
		std::cout << "exception caught = " << ex.what() << "\n";
	}
}

-------------------------------------------------------------------------------------------------

YAN ETKİYE SAHİP BİR KODDA YAZILABİLİR

void operator""_print(const char *p)
{
	std::cout << std::quoted(p);
}

int main()
{
	78945_print; // "78945" yazar.
}

-------------------------------------------------------------------------------------------------

throw 12 ifadesinin türü nedir?

decltype(throw 12) foo(); // mouse ile üzerine gelince void görünüyor.

throw ifadelerinin türü void. 
Value categorysi nedir ? PR Value

-------------------------------------------------------------------------------------------------

int foo()
{
	//return 5; 2 türlüde test edilebilir.
	return -5;
}

int main()
{
	try{
		int x = foo();
		auto val = x > 0 ? x : throw std::runtime_error{"hata"}; // GEÇERLİ
		std::cout << "val = " << val << "\n";
	}
	catch(const std::exception &ex)
	{
		std::cout << "exception caught: " << ex.what() << "\n";
	}
}

-------------------------------------------------------------------------------------------------

C DE OLMAYIP C++ TA OLAN ÖNEMLI BIR KURAL VAR

void func()
{
	return 6; // Sentaks hatası iki dilde de
}


void foo();

void func()
{
	return foo(); // bir void functionun return ifadesi olabilir ama türü void olmak zorunda
}

-------------------------------------------------------------------------------------------------

VOID TÜRDEN IFADE FARKLI GÖRÜNTÜLERDE KARŞIMIZA ÇIKABILIR

int main()
{
	pvcat(void()); // pvcat hocanın ilk derslerde yazdığı koddu
}

decltype(void()) foo();

int foo()
int main()
{
	(void)foo();
}


int main()
{
	int x = 10;

	(x,void());
}

void foo(); // Yukarıdaki örnek
void func()
{
	return foo(); // bir void functionun return ifadesi olabilir ama türü void olmak zorunda
}

-------------------------------------------------------------------------------------------------

CONSTRUCTORU OVERLOAD ETMEK ISTIYORUM AMA IMZA FARKLILIĞI YOK. DOUBLE PARAMETRELI AYRI IKI FUNC VAR.

#include <numbers> //C++20 ile eklendi. Matematiksel sabitler var burada

class Circle {
public:
	explicit Circle(double radius) : m_radius{radius} { };
	explicit Circle(double diameter) : m_radius{diameter} { }; // bu ve yukarıdaki double.NASIL OLACAK ŞİMDİ ?? :D:D:D
	double get_area()const
	{
		//std::numbers::pi_v<double>;
		//std::numbers::pi_v<long double>;
		//std::numbers::pi; // buda geçerli. std::numbers::pi_v<double> ile aynı anlamda bu

		return std::numbers::pi * m_radius * m_radius;
	}
	double get_circumference()const
	{
		return 2 * std::numbers::pi * m_radius * m_radius;
	}
	void set_radius()
	{
		m_radius = radius;
	}
private:
	double m_radius;
};

-------------------------------------------------------------------------------------------------

ÇÖZÜMLERDEN BİRİ. ÇOK KÖTÜ BİR ÇÖZÜM AMA VAR

class as_diameter{};
class as_radius{};

class Circle {
public:
	explicit Circle(as_diameter, double radius) : m_radius{radius} { }; // BURAYA PARAMETRE OLARAK EKLENDİ
	explicit Circle(as_radius, double diameter) : m_radius{diameter} { }; // BURAYADA EKLEDİK
	double get_area()const
	{
		//std::numbers::pi_v<double>;
		//std::numbers::pi_v<long double>;
		//std::numbers::pi; // buda geçerli. std::numbers::pi_v<double> ile aynı anlamda bu

		return std::numbers::pi * m_radius * m_radius;
	}
	double get_circumference()const
	{
		return 2 * std::numbers::pi * m_radius * m_radius;
	}
	void set_radius()
	{
		m_radius = radius;
	}
private:
	double m_radius;
};

int main()
{
	Circle c1{as_diameter{},2.52};
	Circle c2{as_radius{},2.52}; // BU şekilde çağrılabilir.

	std::cout << "c1.get_area() = " << c1.get_area() << "\n";
	std::cout << "c2.get_area() = " << c2.get_area() << "\n";

}

-------------------------------------------------------------------------------------------------

DİĞER ÇÖZÜM İSE NAMED CONSTRUCTOR KULLANABİLİRİZ.
Constructorları private yapıp, constructoru çağıran functionlar yazabiliriz mesela.
BUDA KÖTÜ ÇÖZÜM

class Circle {
	explicit Circle(as_diameter, double radius) : m_radius{radius} { }; 
	explicit Circle(as_radius, double diameter) : m_radius{diameter} { };
public:
	Circle get_circle_with_diameter(double diameter); //bu eklendi
	double get_area()const
	{
		//std::numbers::pi_v<double>;
		//std::numbers::pi_v<long double>;
		//std::numbers::pi; // buda geçerli. std::numbers::pi_v<double> ile aynı anlamda bu

		return std::numbers::pi * m_radius * m_radius;
	}
	double get_circumference()const
	{
		return 2 * std::numbers::pi * m_radius * m_radius;
	}
	void set_radius()
	{
		m_radius = radius;
	}
private:
	double m_radius;
};

-------------------------------------------------------------------------------------------------

BURADA HATALARA KARŞI DAHA SAVUNMA MEKANİZMASI OLUŞTURMAK İÇİN DIAMETER VE RADIUS GİBİ 
DEĞERLERİN DOĞRUDAN KENDİSİNİN TÜR OLMASI. YANİ TÜRÜN KENDİSİNİN İSMİ O TÜR HANGİ VARLIĞA KARŞILIK
GELİYORSA ONU TEMSİL ETMESİ.

BUNUN ÖNEMLŞİ BİR AVANTAJIDA TEMPLATE HALİNE GETİRİLEBİLMELERİ. PROBLEM DOMAININDE BİRSÜRÜ
VARLIK BASİT BİR İNT VEYA DOUBLE IN SARMALANMIŞ HALİ OLABİLİR.

BU KODLAR UYGUN ŞEKİLDE YAZILIRSA, DOUBLE I DOĞRUDAN KULLANMAKLA ONU SARMALAYAN SINIF ARASINDA
MALİYET FARKI OLMAYACAK. STANDART LIBRARYDEKİ ÖRNEĞİ CHRONO LIBRARY.

int main()
{
	using namespace std;
	using namespace std::chrono;
	std::chrono::milliseconds msec;

	msec = 21342ms; //GEÇERLİ
}

BUNLARI OLUŞTURALIM EN PRIMITIVE TURDEN BAŞLAYARAK
Temel C++ Örneği yine

class Kilogram{
public:
	class PreventUsage{};
	constexpr explicit Kilogram(PreventUsage, double val) : m_val{val} { }
	double get()const
	{
		return m_val;
	}

	friend constexpr Kilogram operator+(const Kilogram &left, const Kilogram &right)
	{
		return Kilogram{preventUsage{}, left.m_val + right.m_val};
	}

private:
	double m_val;
};

constexpr Kilogram operator""_kg(long double val) //Temel Tür Kg olacak
{
	return Kilogram{Kilogram::PreventUsage{}, static_cast<double>(val)};
}

constexpr Kilogram operator""_g(long double val)
{
	return Kilogram{Kilogram::PreventUsage{}, static_cast<double>(val / 1000.)};
}

constexpr Kilogram operator""_mg(long double val)
{
	return Kilogram{Kilogram::PreventUsage{}, static_cast<double>(val / 1000. / 1000.)};
}

int main()
{
	constexpr auto x = 12.5_kg + 123.42_g + 1286._mg; // artık kilogram bizim için bir tür.
}														// kg yede double türden değer atayamayız doğrudan

Biraz daha çabayla bunları template hale getirebiliriz.Ona yolumuz var.

MOLA

=============================================================================================================================
=============================================================================================================================
=============================================================================================================================
=============================================================================================================================
=============================================================================================================================

Yukarıdaki sorunu çözmek için devrimci bir örnek vardı.C++ tarihindeki en devrimci önerilerden biri.
Bu 1-2 kere reddedildi. Öneren Walter Brown

typedef double Meter;
typedef double Kilogram;
typedef double Angle;

void foo(Meter);
void foo(Kiogram);
void foo(Angle); // Burada derleyici function redecleration diyecek.

-----------------------------------------------------------------------------------------------------------------------------

REDDEDİLEN ÖNERİ
OPAQUE TYPEDEF

opaque typedef double Meter; // Bir türe bu şekilde eş isim verince bu artık distinc tür olsun
							 
Böyle bir araç olmadığı için strong türler oluşturmak için epey bir template metacodelar oluşturmak gerekiyor.

-----------------------------------------------------------------------------------------------------------------------------

ÖR: KENDİMİZ AŞAĞIDAKİ GİBİ BUNU SAĞLAYABİLİRİZ.

template <typename T, typename Tag>
class NamedType{
public:
	explicit NamedType(const T& Val) : m_val{ val } { }
	explicit NamedType(T&& Val) : m_val{ std::move(val) } { }
	T& get(){return m_val;}
	const T& get()const {return m_val;}
private:
	T m_val;
};


//struct KilogramTag{};
//struct MeterTag{};

//using Kilogram = NamedType<double,KilogramTag>; //Bunları incomplete type ta yapabiliriz.
//using Meter = NamedType<double, MeterTag>; //Bunları incomplete type ta yapabiliriz.

//incomplete type yapıldı
using Meter = NamedType<double,struct MeterTag>;
using Kilogram = NamedType<double,struct KilogramTag>;

//BUNLARI LITERAL OPERATOR FUNCLARIYLA BIRLEŞTIREBILIRIZ.
Kilogram operator""_kg(long double val) // literal type olmadığından constexpr olmadı 
{
	return Kilogram{static_cast<double>(val)};
}

int main()
{
	Kilogram x{12.5};
	Meter m{3.56};

	x = m; //Türler farklı atamada geçersiz oldu

	x = 12.5kg;
}

BUradan sonra templateler daha iyi bilinmeli. Mesela CRTP kullanacağız yada başka durumlarda Mixing oluşturacağız.

-----------------------------------------------------------------------------------------------------------------------------

RATIOYU EKLEYECEĞİZ

template <typename T, typename Tag>
class NamedType{
public:
	explicit NamedType(const T& Val) : m_val{ val } { }
	explicit NamedType(T&& Val) : m_val{ std::move(val) } { }
	T& get(){return m_val;}
	const T& get()const {return m_val;}

	template<typename ratio_2>
	operator NamedType<T, Tag, Ratio_2> //Tür dönüştürme operator func
	{
		return NamedType<T,Tag,Ratio_2>(get() * Ratio::num / Ratio::den * Ratio_2::num / Ratio_2::den); // kesin çalışacak diye birşey yok mantığını anlamak önemli
	}

private:
	T m_val;
};

//incomplete type yapıldı
using Kilogram = NamedType<double,struct MeterTag>;
using Meter = NamedType<double,struct KilogramTag>;

//BUNLARI LITERAL OPERATOR FUNCLARIYLA BIRLEŞTIREBILIRIZ.
Kilogram operator""_kg(long double val) // literal type olmadığından constexpr olmadı
{
	return Kilogram{static_cast<double>(val)};
}

BUrada kısaca ratio anlatıldı.Yazmıyorum ben. TEMEL C++ ta yine gördük.Tamamını gördük orada.
ratio_less, ratio_multiply,ratio_add ... 


=============================================================================================================================

Bu kısımda random sayı üretme kısmına çok kısa girdi hoca
Bunuda yazmıyorum. Ders nosunu yaz

=============================================================================================================================

En büyük sorunlardan biri standart kütüphaneyi öğrenmeden kendi kendine programcının bu kodu yazması.
Standart kütüphane bu konuda çok iyi. Gidip kendimiz yazarsak kodları birçok şeyi eksik yazacağımız açık.

*/
