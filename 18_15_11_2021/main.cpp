/*


CTAD - DEDUCTION GUIDE

Eskiden C++ sınıf şablonlarında tür çıkarımıyoktu açık açık belirtmek gerekiyordu.C++17 ile birlikte CTAD geldi. Bu kod yazmayı kolaylaştırıyor.
Standart library nin birçok sınıfında bundan faydalanabiliyoruz. Array vector list set optional pair... gibi birsürü sınıf.
Bununla ilgili yardımcı bir araçta var. Bazen tür çıkarımı sınıfın tasaraımının istediği şekilde yapılamadığından, 
tür çıkarımının yapılmasını sağlayacak ilave bir bilgi verme sentaksı eklendi. Buna deduction guide deniyor.

Burada template argümanlarının bir kısmının çıkarımını yaptırmak gibi bir şey yok.

template <typename T, typename U>
class Myclass{
public:
	Myclass(T,U);
};

it main()
{
	Myclass<int,double> x(10,4.5); // Geçerli
	Myclass x(10,4.5); // Geçerli
	Myclass<> x(10,4.5); // Sentaks hatası
	Myclass<int> x(10,4.5); // Yine sentaks hatası. DİKKAT!!!!! BU FUNCTION TEMPLATELERDE GEÇERLIYDI.FARK VAR ARADA.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	array x1{1,3,4,7}; // Burada çıkarımlar yapıldı.
	array x2{1,3,4.6,7}; // Dikkat biri double türden. Çıkarım yapılamaz.
	array<int> x3{1,3,4.6,7}; // Sentaks hatası.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

C++ 20 İLE GELEN YENILIK

template <typename T>
struct Nec{
	T x;
};

int main()
{
	Nec nec{12}; // Aggregate type.SENTAKS HATASI VAR
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATE OLMASAYDI

struct Nec{
	int ival;
};

int main()
{
	Nec mydata{12}; // GEÇERLI
	Nec mydata = {12}; // GEÇERLI
	Nec mydata = 12; // GEÇERSIZ
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
struct Data{
	T tval;
};

int main()
{
	Data<int> x{234}; //Geçerli
	Data<double> y{2.34}; //Geçerli
	Data<int> y = 56; //Geçersiz

	Data x{234}; //Ctad yok. Hata
	Data y{2.45}; //Ctad yok. Hata. C++2020 YE KADAR BUNU GEÇERLI KILMAK IÇIN DEDUCTION GUIDE EKLIYORDUK.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

DEDUCTION GUIDE EKLEYELIM

template <typename T>
struct Data{
	T tval;
};

//DEDUCTION GUIDE SENTAKSI
template <typename T>
Data(T) -> Data<T>;

int main()
{
	Data x{234}; //GEÇERLİ ÇÜNKÜ DEDUCTION GUIDE EKLENDİ
	Data y{2.45}; //GEÇERLİ ÇÜNKÜ DEDUCTION GUIDE EKLENDİ
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

DEDUCTION GUIDE I SİLİP C++20 YE ALDIK.

template <typename T>
struct Data{
	T tval;
};

int main()
{
	Data x{234}; //GEÇERLİ ÇÜNKÜ C++20
	Data y{2.45}; //GEÇERLİ ÇÜNKÜ C++20
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

DEDUCTION GUIDE İÇİN İSİMLENDİRİLMİŞ BİR NESNE GEREKSİNİMİ YOK

int main()
{
	vector vec{1,2,3,4,5}; //GEÇERLİ
	
	auto ptr = new vector{1,2,3,4}; //GEÇERLİ

	//YADA FUNCTION STYPE CAST
	auto x = optional(12); // BUNA FUNC STYPE CAST DENİYOR
}

----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------

GITHUB ÖRNEKLERİ

#include <vector>
#include <list>
#include <set>

int main()
{
	using namespace std;

	vector vec{ 1, 2, 4, 7 }; 
	list ls{ 1.2, 2.3, 6.6, 1.9 };
	set s1{ 2, 4, 6, 8 };
	set s2{ {2, 4, 6, 8}, [](int a, int b) {return b < a; } }; // Burada bir lambda ifadesi kullandık.Closure type türünden geçici nesne.
															   // Bu lambdanın türü neyse setin 2. template parametresine karşılık gelen tür argümanı
}															   // yine ctad ile çıkarım yapılacak.

----------------------------------------------------------------------------------------------------------------------------------------------------------

SETI HATIRLAYALIM

bool myfunc(int x, int y);

int main()
{
	set<int,decltype(&myfunc)> myset; // karşılaştırma kriteri burada myfunc.

	myset.insert(12); //Böyle yapılabilir.

	---------------------------------------------------------------------------------------------------------------------------------------

	set<int,decltype(&myfunc)> myset(myfunc); // İlgili Ctor bu. Hatta C++20 ile myset; desek doğrudan ok idi.

	auto f = [](int a, int b){ return a > b;};

	set<int, decltype(f)> myset; //C++17 de SENTAKS HATASI. C++20 DE GEÇERLİ EĞER STATELESS LAMBDA İSE
	set<int, decltype(f)> myset(f); //C++17 de GEÇERLİ

	---------------------------------------------------------------------------------------------------------------------------------------

	int ival;
	auto f = [ival](int a, int b){ return a > b;}; //DİKKAT!!

	set<int, decltype(f)> myset(f); // GEÇERLİ
	set<int, decltype(f)> myset; // C++20 DE BİLE SENTAKS HATASI


}

----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <optional>
#include <atomic>
#include <tuple>
#include <mutex>
#include <thread>
#include <complex>

using namespace std;

mutex mt;

int main()
{
	optional op{ 23 };
	atomic ato{ 56 };
	tuple tx{ 2, 5.6, "ali" };
	lock_guard guard{ mt };
	complex c{ 1.4, 7.87 }; //Hepsi ok
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

FUNCTION TÜRLERINI YAZMAK PROGRAMCILARI ZORLADIĞI IÇIN STD::FUNCTION KULLANIYORLAR.

#include <functional>

int foo(int);
double bar(double, double);

struct Functor {
	void operator()(int);
};

int main()
{
	std::function f1{ foo }; // std::function<int(int)> f1{foo};
	std::function f2{ &foo };
	std::function f3{ bar };
	std::function f4{ Functor{} };
	std::function f5{ [] (int x, int y) {return x + y; }};
}

----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------

FUNCTION TÜRÜYLE FUNC ADRESI TÜRÜ KARIŞTIRILIYOR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

BURADAKİ AÇIKLAMALAR BENCE AŞIRI ÖNEMLİ !!!!!!!!!!!!!!!!!!!!!!!!!!

int foo(int);

int main()
{
	auto x = foo; // int (*)(int). Burada function to pointer conversion / decay var.

	auto &y = foo; // int(int). Burada decay yok. Burada autoya karşılık gelen tür func türü.Yani int(int)
					// y referans olduğundan bu türe referans. int (&y)(int) = foo;

	auto &z = &foo; // SENTAKS HATASI.& operandı olunca ifade pr value expr.
					// Sol taraf referansına sağ taraf değeri atanmaya çalışılmış.
					// auto &&z = &foo; geçerli olurdu. Veya const &r ile tutulabilirdi.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TypeTeller;

template <typename T>
void func(T&)
{
	TypeTeller<T> x;

}

int foo(int);

int main()
{
	func(foo); // Bu klasik hile ile görülüyor türü.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void func(T&)
{
	T f; // DİKKAT. BU FUNC TANIMI DEĞİL BİLDİRİMİ :D:D:D BU YÜZDEN LİNKER HATA VERİR.
	f(12);

	std::cout << typeid(decltype(f)).name() << "\n";
}

int foo(int)
{
	return 1;
}

int main()
{
	func(foo);
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

using ftype = int(int);

int foo(int);

int main()
{
	ftype *fptr = foo; // int(*fptr)(int) = foo; aslında

	std::function<ftype> f{foo};	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

MOST VEXING PARSE

class A{

};

class Myclass{
public:
	Myclass(A);
	void func();
};

int main()
{
	Myclass m(A()); // Burada most vexing parse var.Burada func bildiriminin önceliği var.
					// Func parametresi func olamaz, dizi olamaz... function pointer olur ama func olamaz.
					// Burada m parametresi bir func pointer.

	std::cout << typeid(m).name() << "\n"; // class Myclass __cdecl(class A (__cdecl*)(void))
											// int[] func parametreis yazdığımızda bu bir pointer parametresi oluyoırsa
											// burada da func pointer oluyor.m nin parametresi return değeri A olan parametresi olmayan
											// bir function. Myclass m(A(*)()) oldu yani.

	m.func(); //SENTAKS HATASI

	---------------------------------------------------------------------------------------------------------------------------------------

	Most vexing parse tan kurtarmak için eskiden tekrar parantez içine alınıyordu

	Myclass m((A())); // Eskiden böyle yapılıyordu

	Myclass m(A{}); // böyle yapılabiliyor.
	Myclass m{A()}; // böyle de yapılabiliyor.

}

----------------------------------------------------------------------------------------------------------------------------------------------------------

STD::FUNCTIONDAN CTAD DAN ÇOK İYİ FAYDALANIYOR
Yukarıda github örneği vardı tekjrar yazmıyorum aynı örneği.

void func(int f(int)); // bu funcın parametresi func pointer
void func(int(*f)(int)); // bununla yukarıdaki arasında fark yok.Şuanda redecleration var :D

int foo(int a[][20]);
int foo(int a[10][20]); // ilk [10] içerideki tamsayı dikkate alınmıyor.
int foo(int (*p)[20]); // Aynısı Temel C

int foo(int a[20]);
int foo(int *p); // yine aynı


----------------------------------------------------------------------------------------------------------------------------------------------------------

ÖNEMLİ !!!!!!

std::vector v1{1,2,3}; //CTAD
std::vector v2{v1,v1}; // vector<vector<int>>
std::vector v3{v1}; // vector<int> //DİKKAT !!!!!!!!!!!! Böyle olmasıda daha faydalı.

----------------------------------------------------------------------------------------------------------------------------------------------------------

COMPLEX SAYILAR İLE İLGİLİ SINIF
--------------------------------

#include <complex>

TEMPLATE
--------
namespace std {
	template<typename t>
	class complex {
		constexpr complex(const t& re = t(), const t& im = t());
		//...
	}
};

İki parametresi var. Biri reel diğeri imajinel kısmı için.


int main()
{
	std::complex c1{ 1.4f, 2.2f }; // std::complex<float>
	std::complex c2(2.8, 6.3); // std::complex<double>
	std::complex c3 = 1.2; // std::complex<double>. func 2. parametresi default argument aldığı için geçerli.
	std::complex c4 = { 4.7 }; // std::complex<double>
	//std::complex c5{5,3.3}; // GEÇERSIZ.Türler farklı çünkü
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T, int SIZE>
class MyClass {
public:
	MyClass(T(&)[SIZE]) {
		//...
	}
};

int main()
{
	MyClass mc("necati"); // T => const char SIZE ==> 7
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

TARTIŞMALI KONU

#include <vector>

template<typename... Args>
auto make_vector(const Args&... elems) {
	return std::vector{ elems... };
}

int main()
{
	std::vector<int> v{ 1, 2, 3 };
	auto x1 = make_vector(v, v); // vector<vector<int>>
	auto x2 = make_vector(v); // vector<int> or vector<vector<int>> ? Derleyiciler farklı şekilde ele alabiliyor.
}

//Currently, different compilers handle this differentlyand the issue is under discussion.

----------------------------------------------------------------------------------------------------------------------------------------------------------

GÜZEL ÖRNEK

#include <iostream>

template <typename T>
struct Nec {
	Nec(T) {
		std::cout << "primary template\n";
	}
};

template <>
struct Nec<int> {
	Nec(const double& x)
	{
		std::cout << "specialization for Nec<int>\n";
	}
};

int main()
{
	Nec x{ 324 }; //Çıkarım Primary template e göre yapılır. T int olduğu anlaşılır.Explicit specialization yazar.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

SADECE İSİMLENDİRİLMİŞ NESNELER İÇİN DEĞİL NEW İFADELERİ İLE OLUŞTURULAN İFADELER VE FUNCTION STYLE
CAST İLEDE KULLANILABİLECEĞİNİ GÖRÜYORUZ.

#include <iostream>
#include <mutex>

std::mutex m;

int main()
{
	auto p = new std::pair{ 1, 5.6 };
	auto guard = std::lock_guard(m);
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <vector>

int main()
{
	using namespace std;
	//vector<pair> vec1{ {12, 5.6}, {23, 5.1}, {5, 1.1} }; //Gecersiz.Ctora bakarak çıkarım yapılıyor.Sözkonusu tür
															// buradaki gibi tür argümanı olarak kullanılıyorsa burada çıkarım yok.
	
	vector<pair<int, double>> vec2{ {12, 5.6}, {23, 5.1}, {5, 1.1} }; //Ya böyle yazabiliriz.
	
	vector vec3{ pair<int, double>{12, 5.6}, pair<int, double>{23, 5.1}, pair<int, double>{5, 1.1} }; //Yada böyle ama bu kötü biraz.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template<class T>
struct A {
	A(T, T);
};

int main()
{
	auto pa = new A{ 1, 2 }; //new expression
	auto z = A(1.2, 5.6);  //function-style cast
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T = double>
struct Nec {
	T val;
	Nec() : val() {}
	Nec(T val) : val(val) {}
	// ...
};

Nec nec1{ 10 }; // Nec<int>
Nec nec2;      // Nec<double> varsayılandan geliyor.

----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <utility>
#include <string>

int main()
{
	using namespace std;

	pair x{ 1, 4.5 };  //pair<int, double>
	pair y("ayse"s, "fatma"); // pair<string, const char *>
	pair z = { 7u, 4.5f };  //pair<unsigned, float>
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <list>
#include <string>
#include <vector>
#include <iostream>

int main()
{
	using namespace std;

	list<pair<string, double>> mylist{
		{"necati", 4.25},
		{"saadet", 6.92},
		{"umut", 4.90},
		{"selim", 2.6},
	};

	vector vec(mylist.rbegin(), mylist.rend()); //not braces here

	for (const auto& [name, wage] : vec) {
		cout << name << " " << wage << '\n';
	}
}

BURADA ÇIKARIMIN NASIL YAPILDIĞI DEDUCTION GUIDE İLE İLGİLİ


==========================================================================================================================================================
==========================================================================================================================================================
==========================================================================================================================================================
==========================================================================================================================================================
==========================================================================================================================================================

DEDUCTION GUIDE
---------------

ÖRNEK SENTAKS
-------------
Myclass(const char *) -> Myclass<std::string>;

template <typename T>
class SmartPtr{
public:
	SmartPtr(T*);
};

int main()
{
	SmartPtr ptr(new int); // T Çıkarımı int olarak yapıldı.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class Myclass{
public:
	Myclass(T);
};

int main()
{
	Myclass m1("Necati"); // T const char * yapılacak.
	Myclass<const char *> m2("Necati"); // 
	
	Myclass<string> m2("Necati"); // Burada Myclass<string> olacak
	Myclass m1("Necati"); // Biz burada da Myclass<string> olmasını istiyoruz. !!!!!!!!!
		
}

String literali ile çağrıldığında T türünün const char değil, string açılımı olmasını istersek
bunu biz verebiliyoruz. Bu template yada template olmayan şekilde olabilir.

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class Myclass{
public:
	Myclass(T*);

};


//DEDUCTION GUIDE.EĞER CTORA GÖNDERILEN ARGÜMAN CONST CHAR * ISE SEN BUNU STRING OLARAK YAP
Myclass(const char *) -> Myclass<std::string>;


int main()
{
	Myclass m1("Necati"); // Artık bunun türü Myclass<string>

	Myclass<const char *> m2("Necati"); // Myclass<const char *> explicit verilmiş.
}

Daha çok kullandığımız sınıflarda deduction guide var. Bizim kendimizin bunu yazması çok yüksek ihtimnal değil.

Deduction guide overload sete girmiyor.

----------------------------------------------------------------------------------------------------------------------------------------------------------

GITHUB ÖRNEKLERİ

#include <string>
#include <typeinfo>
#include <iostream>

template<typename T>
struct Nec
{
	T str;
};

C++20 olursa eğer deduction guide yapmasak bile otomatik yapıyor.C++17 de sentaks hatası olurdu.
Nec(const char*)->Nec <std::string>; // Bu deduction guide ile çıkarımı nasıl yapacağını söylemiş olduk.

int main()
{
	Nec nec{ "Necati Ergin" };

	std::cout << std::boolalpha << (typeid(nec) == typeid(Nec<std::string>));
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

DEDUCTION GUIDE TEMPLATE HALİNDE

template<typename T>
class Nec {
public:
	Nec(T param) : mx(param) { }
private:
	T mx;
};

template<typename T>
Nec(T) -> Nec<T>; // deduction guide

Nec x{5 }; // Nec<int> x{5}; // Bunun için deduction guide olmaması, çıkarım yapılamayacağı anlamına gelmiyor.
		   // Ctora int gönderiyoruz burada.

Nec y(3.3); // Nec<double> y(3.3);

auto z = Nec{ 4L}; // auto z = Nec<long> {4L};

//Nec* p = &x; //gecersiz Burada ise bir çıkarım yok.

//Nec n1{ 'A' }, n2{ 23 }; //gecersiz, böyle virgüllerle ayrılan listede herbir çıkarım türü aynı olmalı. Burda aynı değil.

----------------------------------------------------------------------------------------------------------------------------------------------------------

Burada common type traiti kullanılmış.
Common type, metafunca gönderilen türlerin ortak türü olabilecek en büyük tür demek.
Yada 2 türden değişkeninde atanabileceği değişkenin türü.

common_type<int,double>::type burası double
common_type<int,char>::type burası int
common_type_t<int,char> burası int aynısı yukarıdaki ile.


#include <type_traits>
#include <iostream>
#include <string>

template <typename T>
struct Sum {
	T value;
	template <typename ... Types>
	Sum(Types&& ... values) : value{ (values + ...) } {} //Ctor template halinde.
};

template <typename ... Types> // Deduction guide template ve kendiside variadic parametreye sahip.
Sum(Types&& ... ts)->Sum<std::common_type_t<Types...>>;

int main()
{
	Sum s{ 1u, 2.0, 3, 4.0f }; // Common typeları double olacak. Sum<Double> olacak.

	Sum strsum{ std::string{"abc"}, "def" }; // Common type elde edeceği tür string türü.Sum<string> olacak
	
	std::cout << s.value << '\n' << strsum.value << '\n';
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
struct Pair {
	T1	first;
	T2	second;
	Pair(const T1& x, const T2& y) : first{ x }, second{ y } {}
};

// deduction guide for the constructor:
template<typename T1, typename T2>
Pair(T1, T2)->Pair<T1, T2>;

int main()
{
	Pair p1(12, 45);  //Pair<int, int>
	Pair p2("ali", "batuhan");  //Pair<const char *, const char *> (deduction guide)
	//BU DURUMDA FONKSIYON PARAMETRELERI POINTER'A REFERANS OLACAK DIZIYE REFERANS DEĞIL

	---------------------------------------------------------------------------------------------------------------------

	//DEDUCTION GUIDE OLMASAYDI 2. ÖRNEĞE TEKRAR BAKALIM
	Pair p2("ali", "batuhan"); // 1. çıkarım const char [4], 2. çıkarım const char [8]. Ambigiuty 

	---------------------------------------------------------------------------------------------------------------------
	
	Deduction guide olmadığını düşünelim

	Pair p2("ali","can"); // Burada da şöyle bir hata var. first ve second init ederken const uyumsuzluğu var.

}

----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <string>

template<typename T>
struct Nec {
	T val;
};

explicit Nec(const char*)->Nec<std::string>;

int main()
{
	//Nec n1 = { "neco" };	// Geçersiz, deduction guide ignored.Deduction guide explicit olduğundan explicit Ctor gibi çalışıyor.
	
	Nec n2{ "neco" }; // Nec<std::string> n2{"neco"};
	Nec n3 = Nec{ "hello" }; // geçerli
	Nec n4 = { Nec{"hello"} }; // geçerli
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename... Types>
class Tuple {
public:
	constexpr Tuple(const Types&...); //Parametresi referans
	
	template<typename... UTypes> 
	constexpr Tuple(UTypes&&...);
	//...
};

template<typename... Types>
Tuple(Types...)->Tuple<Types...>; // Türün çıkarımını referans değilde değer türü olarak alınmasını sağlıyor.
									
int main()
{
	Tuple tx{ 42, "hello", nullptr }; // int,const char *, nullptr_t açılımı. 
										// Deduction guide olmasaydı parametre türleri referans türü olacaktı

}

----------------------------------------------------------------------------------------------------------------------------------------------------------

vector sınıfının range parametreli constructorunda deductin guide kullanılıyor.
cppreference örneğini yazmadım.	

Library implemente etmiyorsak çokta kullanılmıyor aslında deduction guide.

==========================================================================================================================================================
==========================================================================================================================================================
==========================================================================================================================================================
==========================================================================================================================================================
==========================================================================================================================================================

TEMPLATELERDE FRIEND BİLDİRİMİ
------------------------------
Baya karmaşık bir konu.Bir sınıfın yada bir global funcın yada bir sınıfın member funcın, tanımladığımız
sınıfın private bölümüne erişme hakkı olduğunu belirten bir bildirim.Friendlik doğru bir şekilde kullanılmazsa
data hiding avantajları ortadan kalkabilir. 

class Myclass{
private:
	friend void foo();
	void func();
};

void foo()
{
	Myclass mx;

	mx.func();
}

Mülakat sorusu friend bildirimi data hidingi çiğnemiyor mu?
Java ve C#dillerinden gelenler friend bildiriine kötü bakıyorlar. BUrada diğer dillerden farklı olarak,
bir sınıfın interfacei dendiğinde, sınıfın interface ine sadece sınıfın memberları dahil değil.
Bizde global funclarda sınıf interfaceinin bir parçası

class Myclass{
public:
	void f1();
};

Myclass operator+(const Myclass&, const Myclass&); // buda sınıf interface inin bir parçası.Global func ama sonuçta
													// Bunlar sınıfın private bölüümne erişme yetkisi yok.
													// Sınıfın implementasyonu sınıfın private bölümüne erişmesi lazım
													// bunun için friend bildirimi var.

----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------

TEKRAR YAZALIM. KİMLERE FRIENDLIK VERILEBILIR
1 - GLOBAL FUNC
2 - CLASS
3 - CLASS MEMBER FUNCTION


- Friendlik bildirimi public veya private bölümlerinde olabilir.
- Global bir funca friendlik verdiğimde bu function her ne kadar global bir funca işaret etsede bu bildirim
class ın bulunduğu namespacete visible olmuyor.

ÖR:
class Myclass{
public:
	friend void foo();
	
	friend void func(Myclass);

private:

};

int main()
{
	foo(); //Şuanda bu çağrı yapılamaz. Burada bu functionun bildirimi görülmeli.
	Myclass::foo(); //Zaten geçersiz.

	Myclass mx;
	func(mx); //Burada ADL var Myclass scopeuna da bakar.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

class Myclass{
public:
	void func();

private:
};

void foo(Myclass);

int main()
{
	Myclass mx;
	mx.func(); // çağrılabilir.

	foo(mx); // böylede çağrılır.	 
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

namespace Nec{
	class Myclass{
	public:
		void func();
	private:
	};

	void foo(Myclass);
}

int main()
{
	Nec::Myclass mx;
	mx.func();
	foo(mx); //Geçerli olma sebebi ADL
}

C++ ın ilk dönemlerinde doğrudan çağrılabiliyormuş.

----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------

CLASSA FRIENDLİK VERMEK

class Myclass{
public:
	friend class Nec; // Sınıfa friendlik verilince bu sınıfın daha önceden bildiriminin yapılması veya tanımının görülmesi gerekmiyor
	
	friend void Nec::foo(); // Burada friend geçerli olması için, Nec sınıfının definitionunun friendliğin verildiği noktada visible olması gerekiyor
private:
};

----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------

FRIEND TEMPLATE 
Çok kombinasyon var burada.
Bir sınıf bir func şablonuna friendlik verebilir.
Bir sınıf bir sınıf şablonuna friendlik verebilir.
Bir sınıf bir func/sınıf şablonunun specializationuna friendlik verebilir.
Bir sınıf şablonu bir func şablonunun friendlik verebilir.
Bir sınıf şablonu bir func şablonunun bütün specializationlarına yada sadece ortak template parametresiyle belirlenen friendlik verebilir.


template<typename T>
class Myclass{
public:
	Myclass(T &x){
		std::cout << typeid(T).name() << "\n";
	}
};

int main()
{
	int ival{};

	Myclass<int> x(ival); // T int

	Myclass x(ival); // T int

	-------------------------------------------------------------------------------------------------------------------

	int a[10]{};

	Myclass x(a); // T int[10]

}

----------------------------------------------------------------------------------------------------------------------------------------------------------

DEDUCTION GUIDE EKLEYELİM

template<typename T>
class Myclass{
public:
	Myclass(T &x){
		std::cout << typeid(T).name() << "\n";
	}
};

template <typename T>
Myclass(T) -> Myclass<T>

int main()
{
	int a[10]{};

	Myclass x(a); // Sentaks hatası.Çünkü a decay oldu Myclass<int *> oldu burada. 
				  // Bu durumda func parametresi int * a referans oldu ve sentaks hatası oluştu
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

NORMAL BIR SINIF, BIR SINIF VEYA FUNC ŞABLONUNA FRIENDLIK VEREBILIR

class Myclass{
	template<typename T>
	friend void func(T);

	// friend void func<int>(int);  Buda olabilirdi.int açılımına friendlik vermiş olurduk.

	void foo();

};

template <typename T>
void func(T)
{
	Myclass x;

	x.foo(); // GEÇERLİ.
}

int main()
{
	func(12); //Friendlik olmasaydı sentaks hatasıydı.
}	

----------------------------------------------------------------------------------------------------------------------------------------------------------

BIR SINIF, SINIF ŞABLONUNADA FRIENDLIK VERILEBILIR.

class Myclass{

template<typename T>
friend class Nec;

};

----------------------------------------------------------------------------------------------------------------------------------------------------------

BIR SINIF ŞABLONUNUN, ŞABLON OLMAYAN BİR FUNCA FRIENDLIK VERILEBILIR.

template<typename T>
class Myclass{

	friend void foo(int);

	void pfunc();
};

void foo(int)
{
	Myclass<int>a1;
	Myclass<double> a2;
	Myclass<long> a3;

	a1.pfunc();
	a2.pfunc();
	a3.pfunc(); // Bunları tabi compile ediyoruz run edersek linkerda hata alırız.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

BIR SINIF ŞABLONU, BAŞKA BİR SINIFA FRIENDLIK VEREBİLİR.

template <typename T>
class Myclass{
	friend class Nec;
	void pf();
};

class Nec
{
	void nf1(Myclass<int> x)
	{
		x.pf(); //geçerli
	} 
	
	void nf2(Myclass<double> x)
	{
		x.pf(); //geçerli
	}
};


----------------------------------------------------------------------------------------------------------------------------------------------------------


BIR SINIF ŞABLONU, BİR FUNC TEMPLATE E FRIENDLIK VEREBİLİR.

Dikkat edilmesi gereken yer, derleyicinin bu sınıfın bir sınıf şablonu olduğunu bilmesi gerekiyor.

template<typename T> 
class Nec; //forward decleration gerekti

template<typename T>
class Myclass{
	friend class Nec<T>;

	void foo();
};

template <typename T>
class Nec{
public:
	void f(Myclass<T> x)
	{
		x.foo(); //Geçerli.
	}
};

int main()
{
	Nec<int>nec;
	Myclass<int>m;
	
	nec.f(m); // Geçerli.
}

Burada Nec in int açılımı Myclassın int açılımının private bölümüne erişebilir.
double açılımı double açılımının private bölümüne erişebilir.

----------------------------------------------------------------------------------------------------------------------------------------------------------

//gfunc işlevi A sınıfının tüm açılımlarının (specialization) private bölümüne erişebilir

template <typename T>
class A {
	friend int gfunc();
	void foo(); //private
};

int gfunc()
{
	A<int> ax;
	A<double> bx;
	A<char> cx;

	ax.foo();
	bx.foo();
	cx.foo(); // Friend bildirimleri olmasaydı bunlar sentaks hatası olacaktı.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <class T> // Forward decleration olmasaydı, friendlik bildiriminin kendisi sentaks hatası olacaktı.
void tfunc(T);		// Çünkü derleyicinin bunun bir func template olduğunu anlaması gerekiyor.

template <typename T>
class A
{
	friend void tfunc<T>(T);
	void foo();
};

template <typename T>
void tfunc(T)
{
	A<T> x;		//A<int> için friend void tfunc<int>(int); bildirimi var
	x.foo();   // friendlik olduğundan private kısımdaki foo ya erişilir.
}


int main()
{
	tfunc(10); // functa T int oldu. A<int> oldu.
	tfunc(2.4);
}

Sorulan bir soruyu cevapladı hoca

----------------------------------------------------------------------------------------------------------------------------------------------------------

BURADA MYCLASS SINIFI NEC SINIFININ BÜTÜN AÇILIMLARINA FRIENDLIK VERIYOR

template <typename T>
class Myclass
{
	template <typename U>
	friend class Nec;
};

----------------------------------------------------------------------------------------------------------------------------------------------------------

Burada A sınıfının tüm specializationları Nec sınıfının Tüm specializationlarının private kısmına erişebilir.

template<typename T>
class Nec {
	template<typename U> // DİKKAT!!!! template<typename T> olsaydı Birebir friendlik olacaktı.Yani A<int> Nec<int> private kısmına erişebilir.
	friend class A;		 // Yani aşağıdaki çağrıda bu durumda geçerli olmayacaktı.
private:
	void foo();
};

template <typename T>
class A {
public:
	template <typename U>
	void afunc(Nec<U> nec)
	{
		nec.foo();
	}
};

int main()
{
	A<int> ax;
	Nec<double> dnec;

	ax.afunc(dnec); // GEÇERLİ. ax in nec funcına Nec in double açılımı türünden nesne gönderdim ve private bölüme eriştim.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class X;

template <typename T>
class A
{
	friend class X<T>; // A nın hangi açılımı olursa X inde o açılımına friendlik verilmiş.

	//friend class X<int>; // Burada Sadece X<int> açılımına erişik verdik. Double açılımı buraya erişemez.
private:
	void foo();
};

template <typename T>
class X {
public:
	void func()
	{
		A<double> ax;
		ax.foo();
	}
};

int main()
{
	X<double> a;

	a.func();
}

*/