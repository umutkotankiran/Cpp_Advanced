/*

FOLD EXPRESSIONS C++17 İLE DİLE EKLENDİ
Variadic bir funca gönderilen funclar bir operatörün operandı yapılıyorlar.Yani recursive function yazdırmak yerine 
yada initizalizer list yada dizi yerine duruma göre daha kolay, yazması daha kolay testi daha kolay olan fold expressions kullanmak.

template<typename ...Args>
auto Sum(const Args& ...pack);


Diyelimki parametre pakedindeki parametreler isimleri olsaydı p1, p2,p3,p4 .. olacaktı
Biz fold ifadesiyle p1 + p2 + p3 + p4 ifadesine dçnüşmesini sağlıyoruz.

En tipik kullanımı bu ifadeyi doğrudan return ifadesi yada return ifadesinin bir kısmı yapmak.
Burada karmaşık gelen nokta fold expressions diye geçen ifadenin yazılma biçimi.

unary left fold		( ... + pack )		 (((p1 + p2) + p3) + p4)
unary right fold	( pack + ... )		 (p1 + (p2 + (p3 + p4)))

binary left fold	(+ ... +)			 (value + ... + pack) // elipsis pakedin solunca left fold oldu 
binary right fold	(+ ... +)			 (pack + ... + value)  // elipsis pakedin sağında right fold oldu.

Yani binary olunca işin içine value da girdi. O kadar fark var.Binary fold expressionda iki operatorde aynı olmalı !!!!!!!!!!!
Yani + ise (+ ... +),  / ise (/ ... /) şeklinde olmalı !!!!!!!!!!!!!!!!!!!!!!

---------------------------------------------------------------------------------------------------------------------------------------------

template <typename ...TS>
auto leftfunc(TS ...args)
{
	return (... && args);  // logic ve ortadaki
}

Problemlerden biri, belirli operatorler dışında pakedin boş olması durumu sentaks hatası.

int main()
{
	std::cout << std::boolalpha;

	std::cout << leftfunc(true,true,true); // True
	
	---------------------------------------------------------------------------------------------------------
	
	PAKETTE HİÇ ARGÜMAN OLMASAYDI LOGIC && DE RETURN DEĞERİ TRUE OLACAK

	cout << leftfunc();  //True
}


---------------------------------------------------------------------------------------------------------------------------------------------

LOGIC VEYA İÇİN HİÇ ELEMAN YOKSA FALSE DÖNECEK

template <typename ...TS>
auto leftfunc(TS ...args)
{
	return (... || args);  // logic veya ortadaki
}

Problemlerden biri, belirli operatorler dışında pakedin boş olması durumu sentaks hatası.

int main()
{
	std::cout << std::boolalpha;

	std::cout << leftfunc(); // False
}

---------------------------------------------------------------------------------------------------------------------------------------------

 , DE HİÇ ELEMAN YOKSA SONUÇ VOID. 

template <typename ...TS>
auto leftfunc(TS ...args)
{
	return (... , args); 
}

Problemlerden biri, belirli operatorler dışında pakedin boş olması durumu sentaks hatası.

int main()
{
	std::cout << std::boolalpha;

	std::cout << leftfunc(); // Void döner
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

BINARY FOLD HALİNE GETİRELİM

template <typename ...TS>
auto leftfunc(TS ...args)
{
	return (0 + ... + args);
}

int main()
{
	std::cout << std::boolalpha;

	std::cout << leftfunc(); // SENTAKS HATASI YOK. PAKET BOŞ OLSA BİLE YOK
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

GITHUB ÖRNEKLERİ ÇOK ÖNEMLİ !!!

ÖR:

#include <string> 
#include <iostream>

template<typename... T>	
auto sum_left(T... args)
{
	return (... + args); // Unary left fold
}

template<typename... T>
auto sum_right(T... args)
{
	return (args + ...); // Unary right fold
}

int main()
{
	using namespace std::literals;
	auto x = sum_left(1, 3, 5, 7, 9); // toplamını döndürecek
	std::cout << "x = " << x << "\n";

	auto y = sum_right(1, 3, 5, 7, 9);
	std::cout << "y = " << y << "\n";

	auto sx = sum_left("ali"s, "can", " usta", "oglu"); // Burada durum farklı. 1. işlem string + cstring
							    // ilk işlemden elde edilen değer string. yine string + cstring
							    // sonra yine string + cstring

	std::cout << "sx = " << sx << "\n";
	//auto sy = sum_right("ali"s, "can", " usta", "oglu"); // SENTAKS HATASI ÇÜNKÜ BU RİGHT FOLD. SAĞDAN BAŞLAYACAK TOPLAMAYA
								// İKİ TANE CSTRING TOPLANIYOR.
	
	//Aşağıdaki gibi olsaydı hata yoktu
	auto sy = sum_right("ali"s, "can", " usta"s, "oglu"); //GEÇERLİ
	std::cout << sy;
}

---------------------------------------------------------------------------------------------------------------------------------------------

// ellipsis paketin solunda
// unary left fold
// operator &&

#include <iostream>

template<typename... Args>
bool all(Args... args)
{
	return (... && args);
}

#include <iostream>

int main()
{
	std::cout << std::boolalpha;
	std::cout << all(true, true, true, true) << "\n";  // (((true && true) && true) && true) && true   DEĞERİ TRUE
	std::cout << all(true, true, true, false) << "\n"; // (((true && true) && true) && true) && false  DEĞERİ FALSE
}

NOT : BAZI OPERATORLERIN ANAHTAR ISIMLERI VAR. && YERINE AND YAZMAK GIBI.

int main()
{
	int x = 10;
	int y = 20;

	std::cout << (x bitand y) << "\n"; // bitsel ve bu | 
}

---------------------------------------------------------------------------------------------------------------------------------------------
ÖR:

#include <iostream>

template<typename First, typename ...Args>
void FoldPrintComma(First&& f, Args&&... args)
{
	std::cout << f;
	auto withComma = [](const auto& v) {
		std::cout << ", " << v;
	};

	(..., withComma(std::forward<Args>(args))); // RETURN İFADESİ OLMAK ZORUNDA DEĞİL.PAKET SAĞDA ELİPSİS SOLDA
						    // WITHCOMMA BİR LAMBDA İFAESİNDEN ELDE EDİLMİŞ NESNE
						    // BURADA HEM TÜR HEMDE FUNC PARAM PAKETİ AÇILACAK.
	std::cout << '\n';
}

int main()
{
	FoldPrintComma("hello", 10, 20, 30);
}

çıktı
-----
hello, 10, 20, 30

---------------------------------------------------------------------------------------------------------------------------------------------

bu örnek
unary left fold ile unary right fold arasındaki farkı göstermeye yönelik.


#include <utility>

template<typename ...Args>
auto fdiv_r(Args && ...args)
{
	return (std::forward<Args>(args) / ...);  //unary right fold. (p1 / (p2 / (p3 / p4 )))  tani perfect forwardingte uygulanacak burada.
}

template<typename ...Args>
auto fdiv_l(Args && ...args)
{
	return (... / std::forward<Args>(args)); //unary left fold (((p1 / p2) / p3) / p4)
}

#include <iostream>

int main()
{
	std::cout << fdiv_r(500, 50, 5, 2) << "\n"; // 20
	std::cout << fdiv_l(500, 50, 5, 2) << "\n"; // 1   ARADA FARK OLUŞTU
}

---------------------------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <utility>

//binary left fold

template<typename ...Args>
void fprint(Args&& ...args)
{
	(std::cout << ... << std::forward<Args>(args)) << " " << '\n'; // binary left fold. cout << p1 << p2 << p3 .... forwardı atladık tabi
}									// forward edilmiş halleri gönderiliyor.

#include <string>
#include <bitset>

int main()
{
	std::string name{ "Necati" };

	std::bitset<8> bs{ 15u };

	fprint(name, 12, 4.5, "Tahsin", bs); // 
}

ÇIKTI
-----
Necati124.5Tahsin00001111     // araya boşluk verecek bir yapı yok maalesef.

---------------------------------------------------------------------------------------------------------------------------------------------

// fold expressions
// binary left fold
// ellipsis paketin solunda binary lefy dold

#include <iostream>

template<typename T, typename... Ts>
int subtract(T init, Ts... args)
{
	return (init - ... - args); //Binary left fold
}

int main()
{
	int result = subtract(100, 50, 20, 7); //'result' is (((100 - 50) - 20) - 7) = 23

	std::cout << "result = " << result << "\n";
}

---------------------------------------------------------------------------------------------------------------------------------------------

// binary right fold

#include <utility>
#include <iostream>

// binary right fold
// operator +
// elipsis paketin sağında binary right fold

template<typename ...Args>
auto sum(Args&& ...args)
{
	return (std::forward<Args>(args) + ... + 10);
}

int main()
{
	std::cout << sum(12, 4.5, 50L) << "\n";  //  12 + (4.5 + (50L + 10))
}

---------------------------------------------------------------------------------------------------------------------------------------------

BURASI BOOSTTAKİ HASH TEMPLATE ASLINDA

#include <functional>

template<typename T>
void hashCombine(std::size_t& seed, const T& val)
{
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename... Types>
std::size_t combinedHashValue(const Types&... args) // hash değerlerini combine eden variadic func şablonu.
{
	std::size_t seed = 0; // initial seed
	(..., hashCombine(seed, args)); // chain of hashCombine() calls
	return seed;
}

https://cppinsights.io/ tan inceledim kodu

---------------------------------------------------------------------------------------------------------------------------------------------

#include <vector>

// bu örnekte unary right fold olması ile unary left fold olması sonuç açısından bir fark olusturmuyor

//unary right fold
template<typename T, typename... Args>
void pushback_left(std::vector<T>&v, Args&& ...args)
{
	(v.push_back(std::forward<Args>(args)), ...); // unary right fold
}

//unary left fold
template<typename T, typename... Args>
void pushback_right(std::vector<T>& v, Args&& ...args)
{
	(... , v.push_back(std::forward<Args>(args)));
}

#include <iostream>

int main()
{
	std::vector<int> vec;
	//pushback_left(vec, 2, 3, 5, 7, 11, 13); // 2 3 5 7 11 13 

	pushback_right(vec, 2, 3, 5, 7, 11, 13); //  2 3 5 7 11 13  Birşey değişmedi. Virgül operatorü olduğu için değişen birşey yok.
						// , ün her seferinde sağındaki solundaki ifade çalışıp vector push yapacak.Öncelik soldan sağa yani.
	for (auto i : vec)
		std::cout << i << "\n";
}

---------------------------------------------------------------------------------------------------------------------------------------------

template<typename T, typename... Args>
void pushback_left(std::vector<T>& v, Args ...args)
{
	auto list = {args...}; // bu bir init list

	for(const auto &x : list)
	{
		//dolaşabiliriz.
	}
}

---------------------------------------------------------------------------------------------------------------------------------------------

#include <vector>
#include <iostream>

template <typename T, typename ...TS>
decltype(auto) push_back(std::vector<T>& vec, TS&& ...args)
{
	(vec.push_back(std::forward<TS>(args)), ...);
	return (vec);
}

int main()
{
	std::vector<int> ivec;

	push_back(ivec, 1, 2, 3, 4).push_back(5); // Pushback global push back miş

 		std::cout << i << " ";
}

---------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void func(T x)
{
	foo(10); // sentaks hatası değil.isim bulamıyor

	foo(x); // BUrası sentaks hatası değil.Buradaki x in türü belli değil.2 aşamalı bir lookup yapılıyor.
}			// 1. aşamada isim arandı bulunamadı. 2. faz isim arama instantiation aşamasında yani func yazılma aşamasında yapılacak.

INSTANTIATION NE DEMEK?
Bir template ten derleyici bir ürün elde edecek.Bu ürün bir sınıf bir function bir değişken bir tür eş ismi olabilir.
Derleyicinin nihayi olarak bu süreç başarılı olursa template ten elde edeceği ürün o template in bir specializationu.
Specialization kelimesini explicit veya partial specialization ile karıştırmayalım.

Template ten üretilen herhangibir ürün o template in specializationu. func bir function template ise 
func<int> olmasına yönelik bir func kodu oluşturduğunda bu func templateinin bir specializationu.

Explicit specialization başka bir şey. Sen Primary template ten kod yazma ben sana kodu vereceğim demek.
Partial da benzer şekilde.

Derleyicinin bunu yapma süreci template i instantiate etmek. Bu template ten hareketle derleyicinin kod yazması demek.
Bu kodu yazması için template parametrelerine karşılık gelen argümanları derleyici bilecek ve onu yerine koyup kodu yazacak.
Bu kodu yazma aşamasında sentaks hatası olabilir.	

Sonuç: İnstantiation, template ten derleyicinin gerçek kodu yazması süreci demek.


POINT OF INSTANTIATION : Derleyici bir tempalte i instantiate ederken o template in karşılığı kodun yazılacağı bir yer
seçiyor. Bu yer dilin kurallarına göre veriliyor bunada point of instantiation deniyor.Bu yer önemli.

Bazı kurallara bağlı ama basitçe, func call ın olduğu noktaya en yakın fakat derleyicinin tüm bilgileri elde etti nokta.


SORUYA GERİ GELİRSEK

template <typename T>
void func(T x)
{
	foo(x); // Buradaki x dependent expression. Neden hata değil ? Bu template in instantiate edildiği yerde belkide ADL
}		// ile bu isim bulunacak. 2 faz isim arama yapılacak. birinci fazda instantiationdan bağımsız sentaks hatası olması
		// gereken yerler sentaks hatası olsun. İkinci fazda, Eğer dependent expressiona bağlı ise burada ismin aranması
		// 2. fazda olacak. 2. fazda ismin aranması tamamen ADL e göre.

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

Funca gönderilen argümanlardan kaç tanesi boolean değerde ise count funcında return değeride o olacak.

bool pred(int i)
{
	return i % 2 == 0;
}

template <typename ...Ts>
std::size_t count(Ts ...ts)
{
	return (std::size_t(0) + ... + (pred(ts) ? 1 : 0));	
}

int main()
{
	std::cout << count(4,1,6,3,8,10); // 4 tane çift var. 4 olacak.

	std::cout << count(4,1,6,3,8,10,1,3,8); // 5 tane çift var. 5 olacak.
}

---------------------------------------------------------------------------------------------------------------------------------------------

ÖDEV

#include <iostream>

void print(int x)
{
	std::cout << x << ' ';
}

template <typename ...Ts>
void call(Ts ...args)
{
	 ///gelen tüm argümanlar ile (gönderildikleri sırayla) print işlevi bir kez çağrılacak
}

template <typename ...Ts>
void reverse_call(Ts ...args)
{
	 ///gelen tüm argümanlar ile (gönderildikleri sırayla ters) print işlevi bir kez çağrılacak
}


int main()
{
	 call(1, 2, 3, 4, 5);  //1 2 3 4 5
	 reverse_call(1, 2, 3, 4, 5);  //5 4 3 2 1
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

AŞAĞIDAKİLER TEMEL C++ TA GÖRÜLDÜ

Member Function Pointerlar ile alakalı soru var yazmayacağım.
Burada güzel açıklama yaptı.
std::invoke anlattı kısaca

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

Templateler sözkonusu olsun yada olmasın isimler sözkonusu ise en çok kullanılan terimlerden biri
qualified name ve unqualified name. 
Standartlarda qualified name lookup ve unqualified name lookup diye geçiyor.

Burada kastedilen, bir isim unary yada binary eğer çözünürlük operatörünün operandı olarak kullanılmışsa(A::x),
nokta operatörünün operandı olarak kullanılmışsa(x.y) veya ok operatörünün operandı olarak
kullanılmışsa buna qualified name/nitelenmiş isim diyoruz.
A::x
::x
A.x
A->x

Ama isim bu operatörlerin operandı olmadan kullanılmşsa o zaman unqualified name deniyor.
Neden önemli, dilin kuralları isim arama / name lookup konusunda, name lookupın kurallarını
ismin nitelenmiş olup olmadığına göre yapıyor.

::func(); // Burada ADL devreye girmez mesela
(func)(mx); // Burada da ADL devreye girmiyor.

İsim arama kuralları herşeyden önce nitelenmiş yada nitelenmemiş ismin aranmasında farklı kurallar
getiriyor.

template <typename T>
class Base
{
public:
	void func();
};

template <typename T>
class Der : public Base<T>
{
	void foo()
	{
		func(); // Sentaks hatası oldu. Demekki func ismi için base e bakılmadı
				// İsim arama kuralları karmaşık. 

		this->func(); // BURADA SENTAKS HATASI YOK.
		Base::func(); // BURADA DA SENTAKS HATASI YOK.
	}
};

Demekki ismin nitelenmesi ile nitelenmemesi arasında her yerde ismin aranması açısından farklılık var.

int x = 20;

int main()
{
	int x = 10;
	
	/x  burada isim bulunda ve local scope
	//::x bu ise global namespace te arayacak.
}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

VARIADIC FUNC AÇILIMLARI

Dilin standartları hangi contexte hangi açılımların mümkün olduğu ve nasıl expand edileceğini
dilin standartları ayrıntılı şekilde belirtilmiş.
Diyelim ki variadic parametre varsa nerelerde ve nasıl expansion edildiğini standartlar söylüyor.

template <typename ...TS>
void func(TS ...pack) // İlk expansion func parametre pakedinde.Kurallar elipsis atomu template parametre pakedinden sonra 
{						// geliyorsa burada hertürlü expansion yapılıyor. yani Ts ...pack diyince burada da expansion yapılıyor.
						// T1 p1, T2 p2 ... şeklinde açılıyor.
	
	sizeof...(pack) // burada da pack expansion kabul ediliyor.
	sizeof...(Ts) // burada da pack expansion kabul ediliyor.
}

Pack expansion yapılan yerlere bakılabilir.

---------------------------------------------------------------------------------------------------------------------------------------------

KLASIK SENARYOLAR

1- Bir sınıf şablonunda veya bir func şablonunda argüman listesi olarak kullanılması

template <typename ...Types>
class Myclass{};

template <typename ...TS>
void func(TS ...args)
{
	Myclass<Ts...> x; // Myclass<T1,T2> gibi yani Mclass<int,double> gibi
}

-------------
ÖR:

template <typename ...Types>
class Myclass{
public:
	Myclass()
	{
		std::cout << typeid(Myclass).name() << "\n";
	}
};

template <typename ...Ts>
void func(Ts ...args)
{
	Myclass<Ts ...>x;
	// Myclass<Ts ...>x(args...); // böylede yazabilirdik eğer constructoruda yazsaydık
}

int main()
{
	func(1,2.3,"dogukan"); // class Myclass<int,double,char const *>  
}

----------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>

template<typename ...TS>
class Var {
public:
	Var()
	{
		std::cout << typeid(Var).name() << "\n";
	}
};

template <typename ...Types>
class Myclass : public Var<Types...>  // Myclass<int,double>, Var<int ,double > açılımdan elde edilmiş.
{ 
public:
	constexpr static size_t size = sizeof...(Types); // 2 değerinde
};

int main()
{
	constexpr auto n = Myclass<int, double>::size; // 2
	
	Myclass<int, double, long, char> x;
}

ÇIKTI
-----
class Var<int,double,long,char>

NOT: 
public Var<Types...>,
Burada public X,Y,Z mi yoksa public x, public y, public z mi
ikinci olan public x, public y, public z

Bunu X Y Z içerisine function yazarakta ispatlanabilir hoca 
func bildirimi yaptı ve sentaks hatası işaretlemedi.

----------------------------------------------------------------------------------------------------

#include <iostream>
#include <tuple>

template <typename ...Ts>
class Nec : public std::tuple<Ts...> 
{
	
};

int main()
{
	Nec<int,long,double>; // Bu türü kullanınca buda tuple ın int long double açılımını kullanacak.
}

----------------------------------------------------------------------------------------------------

TYPES*
------

#include <iostream>

template<typename ...TS>
class Var {
public:
	Var()
	{
		std::cout << typeid(Var).name() << '\n';
	}
};

template <typename ...Types>
class Myclass : public Var<Types* ...> { }; // Var<Types ...> yazsaydık int double long yazacaktı

int main()
{
	Myclass<int, double, long> x;
}

Çıktı
-----
class Var<int*, double*, long *>

Var<Types** ...> yazsakdyık
class Var<int**, double**, long **>


----------------------------------------------------------------------------------------------------

TABAN SINIF CTOR İNİT LİSTTE DE KULLAILABİLİR

#include <iostream>

struct X {
	X(int i)
	{
		std::cout << "X(int i) i = " << i << '\n';
	}
};

struct Y {
	Y(int i)
	{
		std::cout << "Y(int i) i = " << i << '\n';
	}
};

struct Z {
	Z(int i)
	{
		std::cout << "Z(int i) i = " << i << '\n';
	}
};

template <typename ...Types>
class A : public Types ... 	// A yı Types tan kalııtmla elde ediliyor.
{ 
public:
	A() : Types{ 0 }... //Base sınıfın Ctoruna 0 ile çağrı yapılmış
	{		    // Multiple inheritance var.X Y ve Z den kalıtımla elde edilmiş.

	}
};


int main()
{
	A<X, Y, Z> ax;
}

ÇIKTI
-----
X(int i) i = 0
Y(int i) i = 0
Z(int i) i = 0

----------------------------------------------------------------------------------------------------

//pack expansion as template argument lists.CPPREFERENCE ÖRNEĞİ

#include <tuple>

template<class A, class B, class... TS>
void func(A arg1, B arg2, TS ...pack)
{
	std::tuple<A, B, TS...> t1;  // std::tuple<A, B, E1, E2, E3>
	std::tuple<TS..., A, B> t2;  // std::tuple<E1,E2,E3,A,B>
	std::tuple<A, TS..., B> t3;  // std::tuple<A,E1,E2,E3,B>
	//...
}

----------------------------------------------------------------------------------------------------

template <std::size_t ...n>
class Myclass{
public:
	Myclass()
	{
		std::cout << typeid(Myclass).name() << "\n";
	}
};

template <std::size_t ...n>
void func()
{
	Myclass<n...> x;
}

int main()
{
	func<1,3,5,7>(); // class Myclass<1,3,5,7>
}

ÇIKTI
-----
class Myclass<1,3,5,7>

----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------

CTAD VE DEDUCTION GUIDE İŞLENECEK SIRAYLA

CTAD
----

using namespace std;
int main()
{
	pair p{12,45}; //CTAD VAR
	
	vector vec{1,3,5,7,9}; // CTAD VAR
	
	optional x{34.5};

	atomic a{90};

	lock_guard guard{m};
	
	complex c{1.4, 9.5};

	initializer_list mylist{1,3,6,7};

	array x{2L, 4L, 8L};

	list ls(vec.begin(), vec.end()); // HEPSİ OK AMA BURADA SORUN VAR GİBİ. LS NİN TÜRÜ LİST<İNT>.
					// ARGÜMAN OLARAK GÖNDERDİĞİM İFADELER VECTOR<INT>::ITERATOR TÜRDEN HALBUKİ
					 // BUNU SAĞLAYAN DEDUCTION GUIDE C++17 !!!!!!!!!!!!!!!!!!!!!!!!!!!

	list ls{vec.begin(), vec.end()}; // NORMAL PARANTEZ YERİNE KÜME PARANTEZİ VERİLDİ.ARGÜMANLAR YİNE VECTOR<INT>::ITERATOR 
					// LS NİN TÜRÜ LİST<VECTOR<INT>::ITERATOR> YANİ VALUE TYPE İTERATOR TÜRÜ OLDU

}

----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------

Burada kısaca bir template tekrarı yazmayacağım.

NOT: SInıfın çağrılmayan member funcları için instantiation yapılmayacak.
NOT: TÜr dönüştürme operator funcları template olabilir.


*/
