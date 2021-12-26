/*

VALUELESS BY EXCEPTION VARIANTTA GEÇIYOR

#include <variant>
#include <stdexcpt>
#include <iostream>

struct Nec{
	operator int()
	{
		throw std::runtime_error{"Hata hata hata"};
		return 1;
	}
};

int main()
{
	using namespace std;

	variant<double, int> var{3.4};

	try{
		var.emplace<1>(Nec{}); // Eski değeri destroy etmek durumunda ama yeni değeride elde edemeden 
								// exception throw edecek.
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << \n;
		std::cout << boolalpha << var.valueless.by_exception() << "\n"; // True dönecek
	}
}


======================================================================================================================================================
======================================================================================================================================================
======================================================================================================================================================
======================================================================================================================================================
======================================================================================================================================================

EXCHANGE
--------
Temel C++ ta anlatılmadı.C++14 eklendi,C++20 ve C++23 te değişiklikler yapıldı.utility header içinde.
Swapa benziyor ama aynı şey değil.Bir değişkene değer atıyoruz ama var olan değeride func return değeri ile elde ediyoruz

x = value; diyoruz diyelim ama x in eski değerinide kullanmak istiyoruz.
Bazen iki değişken kullanmak yerine exchange daha verimli oluyor.
C++20 ile birçok function constexpr değildi ama C++23 ile constexpr oldu.

C++ ta herşey compile time a kayıyor.

C++14 TE TEMPLATE AŞAĞIDAKI GIBI
--------------------------------
template <typename T, typename U = T>
T Exchange(T &x, U&& new_value)
{
	T old_value = std::move(x);
	x = std::forward<U>(new_value);
	return old_value;
}

C++17 DE AŞAĞIDAKI GIBI
-----------------------
template <typename T, typename U = T>
constexpt T Exchange(T &x, U&& new_value);

C++20 DE BİR FUNCTİON BİR KOŞULA BAĞLI OLARAK NOEXCEPT YAPILDI
--------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------------------------------------

SINIFLARIN MOVE CTOR VE MOVE ASSIGNMENTTA YOĞUN OLARAK KULLANILIYOR

struct Nec
{
	int mval;
	
	Nec(Nec& other)noexcept : mval{std::exchange(other.mval,0)}{ }
	
	Nec& operator=(Nec &&other) noexcept
	{
		if(this != &other)
		{
			mval = std::exchange(other.mval,0);
		}
		return *this;
	}
};

Move Ctorda
mval, other.mval'in değerini aldı
other.mval burada 0 olacak.Çünkü taşındı.

Move Assignment
other.mval = 0 yapıldı
other.mval in eski değeride mval'e atandı.

------------------------------------------------------------------------------------------------------------------------------------------------------

IOSTREAMDEKİ FLAGS FUNCIN İLKEL VE BENZER OLANINI YAZDIK

#include <utility>

class Stream{
public:
	using flags_type = int;
	flags_type flags()const
	{
		return m_flags;
	}

	flags_type flags(flags_type val)
	{
		return std::exchange(m_flags,val); // m_flags e yeni değerini yazdırdı ama atama yapılmadan önceki 
	}										// değerini return etti.

private:
	flags_type m_flags{};

};


======================================================================================================================================================
======================================================================================================================================================
======================================================================================================================================================
======================================================================================================================================================

USING ENUM BİLDİRİMLERİ
-----------------------
Dan Saks

enum class rgba_color_channel {red,green,blue,alpha};

std::string_view to_string1(rgba_color_channel channel)
{
	switch(channel)
	{
	case rgba_color_channel::red : return "red";
	case rgba_color_channel::green : return "green";
	case rgba_color_channel::blue : return "blue";
	case rgba_color_channel::alpha : return "alpha";	
	}
}

BURADA SIKINTI OLAN UZUN KODUN UZUN OLMASI.DAHA BASİT YAZILABİLİR
USING BİLDİRİMİ İLE DAHA BASİT YAZILABİLİR.

std::string_view to_string2(rgba_color_channel channel)
{
	using enum rgba_color_channel;   // BİLDİRİME DİKKAT !!!!

	switch(channel)
	{
	case red : return "red";
	case green : return "green";
	case blue : return "blue";
	case alpha : return "alpha";	
	}
}

-------------------------------------------------------------------------------------------------------------------

DIYELIMLI ENUM CLASS DEĞİL ESKİ ENUM

Enum türlerinin 3 dezavantajı
- Bildirimini nerede yaparsak o scope a giriyor isimler.Ya nested type yada namespace içine alınıyor
- Underlying type.Derleyiciye bağlı bu. Bu modern C++ta değişti.Artık type belirtilebiliyor.
  Böylece forward decleration yapılabiliyor.
- Aritmetik türlerden enum türlere dönüşüm yok ama enum türlerden aritmetik türlere dönüşüm var
  Bu olmaması gerekiyor. 3. derste bu var.

ÖR:
enum class rgba_color_channel {red,green,blue,alpha};

int main()
{
	auto red = rgba_color_channel::red;

	----------------------------------------------------
	
	RED I DOĞRUDAN KULLANABILIRIZ.

	using rgba_color_channel::red;

	red;

	DİĞER İSİMLERİ NİTELEMEM GEREKİR BURADA.

}

-----------------------------------------------------------------------------------------------------------------------------

GELENEKSEL ENUM İÇİNDE KULLANABİLİRİZ AŞAĞIDAKİ GİBİ.YENİ SCOPED ENUM DEĞİL AMA MYCLASS SCOPEUNDA.

class Myclass{
public:
	enum Color {red, green,blue};
};

int main()
{
	using enum Myclass::color;
	auto c = green;
	auto d = red;
}

======================================================================================================================================================
======================================================================================================================================================
======================================================================================================================================================

DESIGNATED INITIALIZER
----------------------
C++20
Buradaki sentaks C den daha katı. C de geçerli olan epey bir durum C++ ta geçersiz.
Diziler aggregate types ın bir üyesi. C++ ta arraylerde designated init sentaksı yok.

C de ki sentaksa bakalım.C99 da gelmişti.

int main()
{
	int a[10] = {[8] = 9, [3] = 3}; // sıranın bir önemi yok

	int a[10] = {1,2,[7] = 4};

	int a[] = {
		[4] = 54,
		[17] = 423;	
	};				//18 boyutlu dizi

	int a[3][8] = {
	[3] = {1,2,3,4,5},
	[4] = {[2]= 4, [5] = 5}
	};

}

---------------------------------------------------------------------------------------------------------------------------

C DE YAPILARDA DA KULLANILIYOR

struct Date{
	int a,b,c;
	char str[20];
};

int main()
{
	struct Date mydata = {.b = 40};

	struct Date mydate = {5,12,34,"Alican"};

	struct Date mydate = {.a = 3, .b = 5, .c = 6, .str = "asdasd"};
}

---------------------------------------------------------------------------------------------------------------------------

C++20 YE GELELIM

C++ AGGREGATE TYPE DENEN GRUP IÇIN KULLANILIYOR.
Bir aggregate/bileşik, Ya dizi türü olacak, ama sınıfta olabilir. Sınıf olduğunda private veya protected
nonstatic data member olmayacak(tüm öğeler public olacak), inherited ctor olmayacak, user declared ctor olmayacak
, virtual taban sınıf olmayacak, private ve protected taban sınıfı olmayacak ve virtual functionu olmayacak...
Basitçe C deki yapılar gibi olan sınıflar. C++17 ve 20 ile kurallar genişletildi.
Static veri elemanı olması aggregate olmasını engellemiyor

CPPREFERENCE TA BAYA UZUN ÖZELLİKLERİ VAR
https://en.cppreference.com/w/cpp/language/aggregate_initialization

ÖR:
struct Point
{
	double x;
	double y;
	double z;
};

int main()
{
	Point p;

	p.x = 3.4;

	------------------------------------------------------------------------------------------------

	Point p = {.x = 3, .y = 5, .z = 5}; // SIRA DEĞİŞTİRİLEMİYOR !!!!!!!!!! 
										// C DE DEĞİŞTİRİLEBİLİYOR !!!!!!!!!!!!!!!!

	------------------------------------------------------------------------------------------------

	std::vector<Point> pvec;

	pvec.push_back(Point{.x = 1.1, .y = 2.2, .z = 3.3}); // LEGAL
	pvec.push_back(Point{.x = {1.1}, .y = {2.2}, .z = {3.3} }); // {} LEGAL  AMA () kullanılamıyor

}

---------------------------------------------------------------------------------------------------------------------------------

struct Point
{
	double year;
	double mon;
	double day;
};


int main()
{
	Date mydate{ .year = 1987, .mon = 5, .day = 3};
}

---------------------------------------------------------------------------------------------------------------------------------

DEFAULT MEMBER INIT YADA INCLASS INIT KULLANILMIŞSA İLK DEĞER VERMEDİYSEK O DEĞERLERİ VERMİŞ OLUYORUZ

NOT : İLK DEĞER VERME SIRASI DEĞİŞTİRİLEMİYOR AMA ATLAMA/SKIP YAPILABİLİR

struct Point
{
	double x;
	double y = 4.1;
	double z;
};

int main()
{
	Point p = {.z = 5.5};
	std::cout << "x = " << p.x << " y = " << p.y << " z = " << p.z << "\n";
	// 0 4.1 5.5
	------------------------------------------------------------------------------

	Point p = {.y = 3.3 , .z = 5.5};
	std::cout << "x = " << p.x << " y = " << p.y << " z = " << p.z << "\n";
	// 0 3.3 5.5
}

--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------


C İLE ÖNEMLİ FARKLILIKLAR

1. C++ ta arrayler için bu sentaks yok,
2. Sıra önemli, C de sıra önemsiz.
3. Normal ilkdeğer verme sentaksı ile designated init sentaksı birarada C de var C++ ta beraber kullanılamaz.
4. Veri elemanının kendiside bir aggregate ise aggregate olan veri elemanının veri elemanına aşağıdaki gibi değer verilebilir.

struct Time
{
	int min;
	int hour;
};

struct Date
{
	int year;
	int month;
	int day;
	Time time;

	static int hmode;    // Static veri elemanı olması aggregate olmasını engellemiyor
};


int main()
{
	Date d1 = {.hmode = 0}; // SENTAKS HATASI. Static veri elemanına ilk değer verilemez

	Date x = {.month = 3, .year = 1998}; //SENTAKS HATASI SIRA TUTMUYOR

	Date x = {3, .year = 1998}; //SENTAKS HATASI. MIX EDİLEMİYOR

	Date x = {
		.time.min = 25; // SENTAKS HATASI. NESTED MEMBER ACCESS İS NOW ALLOWED.NESTED INIT LIST KULLAN DİYOR.
	};
	
	Date x = {.time = {32, 4} }; // BURASI GEÇERLİ !!!!!!!!!
}


--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------

C VE C++ KIYASLAMASI

struct A {int x, y;};
struct A {struct A a;};

int main()
{
	struct A a = {.y = 1, .x = 3}; // C de geçerli, C++ ta değil

	int ar[] = {[2] = 6 }; // C de legal, C++ ta değil

	struct B b = {.a.x = 10}; // C de legal C++ legal değil

	struct A a2 = {.x = 5, 34}; // C de legal, C++ ta değil

	struct A a3 = {.x = 5, .x = 42 }; // C de legal, C++ ta değil


}

UNIONLARDA DA ILK EĞER VERILEBILIR.

union Nec{
	int a;
	double b;
};

int main()
{
	Nec n1 = {.a = 45};// legal
	Nec n2 = {.b = 1.3}; // legal
	Nec n2 = {a. = 34, .b = 1.3}; // SENTAKS HATASI
}

--------------------------------------------------------------------------------------------------------------------------------

struct ScreenConfig
{
	bool scale = false;
	bool full_screen_mode{false};
	int bits {32};
	int plane{3};
};

ScreenConfig myconfig = {true,false, 64,2}; // burada isimler kullanıldığında kodun okunması daha kolaydı


MOLA


=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

FORMAT KÜTÜPHANESİ
------------------
Outputu yapılacak verinin formatlanması ile ilgili bir sınıf.C++20 ile eklendi

Formalamda neye önem veririz.
Type safe olması
Efficient olması
extensible for user defined types yaz bunları 
kodun kolay okunması

PRINTF grubu formatlama type safe değil.Dezavantaj
printf çok hızlı. Avantaj.
User defined türler için kullanma olanağı yok.

IOSTREAM e gelirsek. Bu kütüphane baştan aşağı kötü tasarım.
Hantal, okunması zor.
User defined types için extensible
type safe

---------------------------------------------------------------------------------------------------------------------------------------

FORMAT NASIL?
-------------
type safe
efficient
user defined turler için extensible
formatlama stringi ile argümanları birbirinden ayrılması
pozisyonel indexleme yapılabilmesi
.. Herşey var.

Header file format
Bu bir function.Parametresi stringview değil ama compile time dastring vieva dönüştürülebilecek bir tür olmalı.

std::format() çağrılınca bir string elde ediyoruz. bu formatlanmış bir string. bunun return değeri olan
stringi istediğimiz gibi kullanabiliriz.

#include<format>

int main()
{
	auto s = std::format(); // return değeri olan stringi tutabiliriz.
	std::cout << std::format(); // yada bu stringi yazdırabiliriz.
}

-------------------------------------------------------------------------------------------------------------------

Format functionu bizden bir string literali istiyor. Biz aslında string vereceğiz.

int main()
{
	using namespace std;

	std::cout << std::format("kadir altindag C++ programcisidir") << "\n"; // böyle kullanılabilir ama bunu yapmayacağız tabi :D

	----------------------------------------------------------------------------------------------
	
	std::cout << std::format("{} {} {}") << "\n"; // {} lar placeholder.Bunlarla eşleşecek argüman gelecek aşağıdakli gibi

	----------------------------------------------------------------------------------------------

	int x = 23;
	double y = 4.5;

	std::cout << std::format("{} {} {}",x,y,"ali") << "\n"; 

	----------------------------------------------------------------------------------------------

	{} IÇINE FORMATLAMA ÖZELLIKLERI YAZILMAZSA DEFAULT FORMATLAMA ÖZELLIKLERI KULLANILIR.

	string name {"Necati"};
	int cnt{20};
	
	std::cout << std::format("{} kisisinin {} kitabi var", name,cnt) << "\n";  // default formatlama özelliği tamsayılar için 10luk sayı sistemi
	
	----------------------------------------------------------------------------------------------
	
	FORMAT SADECE STRING VERIYOR. ÇIKIŞ AKIMINA BIZ VERECEĞIZ.

	std::cout << format("{} {}\n","merhaba","dunya"); // merhaba dunya
	
	----------------------------------------------------------------------------------------------
	
	INDEX BELİRTEBİLİRİZ !!!!
	
	string name {"Necati"};
	int cnt{20};

	std::cout << std::format("{1} kişisinin {0} kitabi var\n",cnt,name) << "\n"; // Necati 20
	std::cout << std::format("{} kişisinin {} kitabi var\n",cnt,name) << "\n"; // 20 Necati
	
	----------------------------------------------------------------------------------------------
	
	AYNI ARGÜMANI BİRDEN ÇOK KEZ KULLANABİLİRİZ.

	int x = 47802;

	std::cout << format("{0} {0} {0}", x); // tüm placeholderlar aynı argümanla ilişkilendirilmiş
	
	----------------------------------------------------------------------------------------------
	
	DİKKAT !!!!!!!!
	BINARY, HEXADECIMAL, DECIMAL, OCTAL SAYI SISTEMI. BIRŞEY YAZMAZSAK DECIMAL YINE

	int x = 47802;

	std::cout << format("{0:b} {0:X} {0:d} {0:o}, ", x); // tüm placeholderlar aynı argümanla ilişkilendirilmiş
	// 101010101..10 BABA 47802 135272
}

=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

FORMATLAMA
----------
{} ---> Default formatlama özelliği ile bir sonraki argüman
{n} -----> n indexli argüman
{2} -----> 2 indexli argüman yani
{:fmt} -----> bir sonraki argüman bu formatlama nitelikleriyle formatlanacak
{3:fmt} -----> 3 indexi ve formatlama niteliği


AŞAĞIDAKİ SIRAYA UYMAK GEREK AMA ÇOĞU OPSIYONEL
-----------------------------------------------
Genel bir pattern var. Sıralamaya uymak gerekiyor.
Küme parantezi içine [] koymuyoruz.bu formatlamaya dahil değil.Ayırmak için koyuyoruz şuanda | ta yazılabilir.

{n:fmt} fmt format oluyor burada
[fill][align][sign][#][0][width][.prec][L][type]

[fill] doldurma karakteri
[align] hizalamaya ilişkin karkater
[sign] işaret bilgisi + -
[#] formatlamanın niteliğini belirli formatlama özellikleri için değiştiriyor. 16 lık sayı sisteminde sayı sisteminin
	tabanının gösterilmesini kullanmak için mesela. Formatlamada tamamlayıcı bilgi.#kullanınca başka seçenek, kullanmayınca defaultu kullanılacak.
[0] padding zero.Doldurma karakteri değil. Yazma alanı genişliğinin başına 0 karakteri konulmasıyla alakalı
[width] yazma alanı genişliği
[.prec] reel sayıların formatlanmasında mesela precision değeri
[L] lokalizasyon özellikleri kullanılacak mı
[type] tür bilgisi


ÖRNEKLER 

int main()
{
	int x = 47802;

	std::cout << format("{} {} {}\n","Arda","Ozlem","Melih");  // arda özlem melih

	-------------------------------------------------------------------------------

	std::cout << format("{1} {0} {2}\n","Arda","Ozlem","Melih"); // Ozlem Arda Melih 

	-------------------------------------------------------------------------------

	std::cout << format("{2} {1} {0}\n","Arda","Ozlem","Melih"); // Melih Ozlem Arda

	-------------------------------------------------------------------------------

	std::cout << format("{0:b} {0:x} {0:o}\n",x);	// 10101..101 baba 135272 
	
	-------------------------------------------------------------------------------
	
	FAZLA OLAN ARGÜMAN HATA DEĞİL

	std::cout << format("{2} {1} {0}\n","Arda","Ozlem","Melih","Tuncay"); //Tuncayı almaz

	-------------------------------------------------------------------------------

	POZİSYONEL İNDEXLEME İLE MANUEL İNDEXLEME BİRARADA KULLANILAMAZ !!!!

	std::cout << format("{2} { } {0}\n","Arda","Ozlem","Melih"); // Compiletimeda bulunan hatalar sentaks hatası
					                             // Runtimeda bulunanlar exception throw eder.
}


MOLA


=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

Formata constant expression göndermeliyiz.
vformat fiye bir func var. Buna runtime ifadesi gönderebiliriz.Görülecek

SIRALAMAYA DİKKAT!!!
[fill][align][sign][#][0][width][.prec][L][type]

int main()
{
	int x = 7238423;

	std::cout << format("|{0}|, |{0:d}|\n",x); // Fark yok aynı çıktı.Decimal ikiside.
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("|{0:x}|, |{0:X}|\n",x); //  6e7317  6E7317

	-----------------------------------------------------------------------------------------------

	std::cout << format("|{0:#x}|, |{0:#X}|\n",x); // # sayı sistemi tabanı. 0x6e7317  0x6E7317 

	-----------------------------------------------------------------------------------------------

	std::cout << format("|{0:o}|, |{0:#O}|\n",x); // Octal.   33471427    033471427   
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("|{0:b}|, |{0:#b}|, |{0:#B}|\n",x); // binary mod. 101101010   0b101101010  0B101101010

	-----------------------------------------------------------------------------------------------

	std::cout << format("{}  {}\n",true,false); // true false yazdı.Artık 0 ve 1 değil :D
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("{:s}  {:s}\n",true,false); // true false yazdı yine. s string anlamında
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("{:b}  {:b}\n",true,false); // Binary formatlamada 1   0   yazar
	
	-----------------------------------------------------------------------------------------------
	
	std::cout << format("{:#b}  {:#b}\n",true,false); // 0b1  0b1
	std::cout << format("{:#B}  {:#B}\n",true,false); // 0B1  0B1
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("{:c}  {:c}\n",true,false); // bu durumda 1 ve 0 ın karakter değeri çıkacak
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("{:x}  {:x}\n",true,false); // hex. 1   0 
	
	-----------------------------------------------------------------------------------------------

	std::cout << format("{:#x}  {:#x}\n",true,false); // hex ve tabanı yazdık.   0x1	 0x0
	std::cout << format("{:#X}  {:#X}\n",true,false); // hex ve tabanı yazdık.   0X1	 0X0

}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

SIRALAMAYA DİKKAT
[fill][align][sign][#][0][width][.prec][L][type]

int main()
{
	double dval = 1545687234.5213213214325678912;

	cout << format("{}\n",dval); //default formatlama. 1545687234.5213213214325678912

	-----------------------------------------------------------------------------------------------

	cout << format("{:f}\n",dval); // fixed notasyon. 1545687234.521321

	-----------------------------------------------------------------------------------------------
	
	cout << format("{:E}\n",dval); // üstel notasyon. e yazsaydık e de küçük olurdu. 1.545E+12 gibir bir sayı 

	-----------------------------------------------------------------------------------------------

	std::cout << format("{}\n", "necati ergin"); // String s olduğundan zaten s yi kullanmasakta olur.

	std::cout << format("{:s}\n", "necati ergin"); //  necati ergin
	
	std::cout << format("{:s}\n", "10 > 5"); // s stringler ve booleanlar için kullanılıyor.

	-----------------------------------------------------------------------------------------------

	int x = 7894;

	std::cout << format("{:18} {}", x,"ali") << "\n"; // |              2564 ali|
							  // x i 18 karakterlik yazla alanına yazacak.
							  // fill character boşluk defaultta

	-----------------------------------------------------------------------------------------------
	
	std::cout << format("|{:<18}| {}",x,"ali"); // |2564              | ali
						    // x i 18 karakterlik alanda sola dayalı olarak yazdır.

	-----------------------------------------------------------------------------------------------

	std::cout << format("|{:>18}| {}",x,"ali"); 	// |              2564| ali
							// x i 18 karakterlik alanda sağa dayalı olarak yazdır.

	-----------------------------------------------------------------------------------------------
	
	std::cout << format("|{:^18}| |{}|",x,"ali");   // |       2564       | |ali|
							// x i 18 karakterlik alanda ortada yazar.
	
	-----------------------------------------------------------------------------------------------
	
	DOLDURMA KARAKTERİ
	------------------
	
	YAZARKEN BUNLARI EN BAŞTAKI ÖNCELIK SIRALAMASINA GÖRE YAPILIYOR

	std::cout << format("|{:!<18}| {}",x,"ali");    // |2564!!!!!!!!!!!!!!| ali
							// doldurma karakteri !

	-----------------------------------------------------------------------------------------------

	std::cout << format("|{:$>18}| {}",x,"ali"); // |$$$$$$$$$$$$$$2564| ali
						     
	-----------------------------------------------------------------------------------------------

	std::cout << format("|{:.^18}| |{}|",x,"ali");  // |.......2564.......| |ali|
							// doldurma karakteri .
	
	-----------------------------------------------------------------------------------------------
	
	NOT : Yazma alanı genişliği yazılacak karakterden küçükse budama yapılmaz.

	std::cout << format("|{:6}|","NECATI ERGIN"); // |NECATI ERGIN|
	
	-----------------------------------------------------------------------------------------------
	
	DİKKAT !!!!!!!!!
	YAZILACAK KARAKTER SAYISINI BELİRLEMEK İÇİN PRECISION KULLANACAĞIZ
	
	int x = 6;  
	std::cout << format("|{:.6|","NECATI ERGIN",x); // NECAT
							 
	HATTA BUNU NESTED OLARAK KULLANALIM!!!!!!!!!!!!!!				 
							 
	std::cout << format("|{:.{}}|","NECATI ERGIN",x); // NECAT
							 
	-----------------------------------------------------------------------------------------------

	int x = 762374;
	std::cout << format("|{:15x}|",x); // 15 karakterlik alana hex yazacak. |          ba206| 

	std::cout << format("|{:15X}|",x); // 15 karakterlik alana hex yazacak ama büyük harflerle. |          BA206|

	std::cout << format("|{:<#15X}|",x); // 15 karakterlik alana sola dayalı hex baseini de beraber. |0XBA206        |

	std::cout << format("|{:_<#15x}|",x); // 15 karakterlik alana sola dayalı hex doldurma alanı. |0xba206________|
	
}

------------------------------------------------------------------------------------------------------------------------

[fill][align][sign][#][0][width][.prec][L][type]

İŞARET ÖZELLİĞİ
---------------

int main()
{
	std::cout << format("|{0}|\n",94); // 94

	------------------------------------------------------------------------------------------------

	std::cout << format("|{0}|\n",-94); // -94
	
	------------------------------------------------------------------------------------------------

	std::cout << format("|{0}| |{0:-}|\n",94); // DİKKAT!!!!! Bu - yazılacak demek eğer sayı - ise.
						   // sayı pozitif ise - yazmaz
						   // CEVAP : 94  94

	------------------------------------------------------------------------------------------------

	std::cout << format("|{0}| |{0:-}|\n",-94); // DİKKAT !!!! Sayı negatif olduğundan - zaten her durumda gösterilecek zaten
						    // CEVAP : -94   -94
						    
	------------------------------------------------------------------------------------------------
	
	Boşluk karakteri gelirse pozitif sayı gelirse + yı kullanmayacak ama bir boşluk bırakacak.
	u boşlğun çıkma sebebi boşluk karakteri
	
	std::cout << format("|{0}| |{0: }|\n", 94); // CEVAP : |94| | 94|  
		
	DİKKAT!!!!
	std::cout << format("|{0}| |{1: }|\n", 94,-45); // CEVAP : | 94| |-45|
	
	Bunlar lat alta yazıldığında sayılar alt alta gelmesini sağlıyor.
	
}

*/
