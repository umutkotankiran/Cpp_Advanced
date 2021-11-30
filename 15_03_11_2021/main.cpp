/*

Derleyiciler çıkarım yapıyor genel olarak. Birde burada bir yöntem var türü belirleme yöntemi.

Scott Meyers Kitabından bu meşhur idiyom

template <typename>
class TypeTeller;

template <typename T>
void func(T)		//Bununlada olur  
{
	TypeTeller<T> x;
}

-------------------------------------------------------------------------------------------

template <typename>
class TypeTeller;

template <typename T>
void func(T&&)
{
	TypeTeller<T> x;
}

int main()
{
	func(10);
}

Derleyicinin output kısmında tür yazıyor zaten.Temel C++ta gördük

-------------------------------------------------------------------------------------------

VARIADIC YAPTIK PARAMETREYI

template <typename...>
class TypeTeller;

template <typename ...Args>
void func(Args&&...)
{
	TypeTeller<Args...> x;
}

int main()
{
	int ival{};

	func(ival, 1.2, "murat");
}

Derleyici yine sentaks hatası verecek.

Yine output kısmında görülür
'TypeTeller<int &,double,const char (&)[6]>'

===========================================================================================
===========================================================================================
===========================================================================================
===========================================================================================
===========================================================================================

IF CONSTEXPR 
------------
C++17 de geldi.Bu compile time if.Derleyici compile timeda bir veya daha fazla koşulu değerlendirip yazdığımız kodun
bir kısmını derleme işleminde kullanıp belki başka kısımlarını kullanmıyor.
Kullanımı Template kodlar ile kısıtlı deil ama genelde template kodlarda kullanılıyor.

IF CONSTEXPR ILE COMPILE TIME DA KOD SEÇIMI YAPILIYOR !!!!!!!!!!!!!!!!!!!

template <typename T>
void func(T x)
{
	//Buraya 2 ayrı kod koymak istiyoruz ama kodlardan biri T tamsayı olması durumunda
	//Diğeri T tamsayı olmaması durumunda çalışacak.
	//Bunlar sfinae, tag dispatch ile yapıladabilir ama if constexpr bu konuda daha kolay kod yazdırıyor.
	//Runtime if e benzesede görüntü, durum öyle değil.
	//Derleyici bu template ten kod üretirken koşulu değerlendirecek, doğru ise if e girecek
	//yanlış ise else kısmına girecek.

	if constexpr(sizeof(T) > 2)
	{
		std::cout << "True path\n";
	}
	else
	{
		std::cout << "false path\n";
	}
}

int main()
{
	func(12); // T int oldu. if constexpr nin doğru kısmına girecek. int bizde 4 byte.
			  // derleyici için içindeki kısmı instantiate etti. Yanlış kısmını instantiate etmedi.

	func('A'); // Şimdi else kısmına girdi false
}

-------------------------------------------------------------------------------------------

ÖR:

template <typename T>
void func(T x)
{
	if constexpr(std::is_integral_v<T>)
	{
		std::cout << "tamsayi turleri icin\n";
	}
	else if constexpr(std::is_floating_point_v<T>)
	{
		std::cout << "Reelsayi turleri icin\n";
		
		//*x = 234; Bunu yazınca compile time hiçbir sentaks hatası yok.Bu true path içinde değilse
					buradaki ifadeler için sentaks kontrolü değilde kısıtlı bir kontrol yapılıyor.
	}
	else
	{
		std::cout << "diger turler\n";
	}
}

int main()
{
	func('A'); // tamsayı türler
	func(1.2F); // reel sayi türler
	func("Ali"); // diğer turler
}

-------------------------------------------------------------------------------------------

ÖR

#include <string>
#include <type_traits>

template <typename T>
std::string as_string(T x)
{
	if constexpr (std::is_same_v<T, std::string>) {
		return x;
	}
	else if constexpr (std::is_arithmetic_v<T>) {
		return std::to_string(x);
	}
	else {
		return std::string(x);
	}
}

#include <iostream>

int main()
{
	std::cout << as_string(42) << '\n'; // 2. koda girecek
	std::cout << as_string(4.2) << '\n';  // 2. kod yine
	std::cout << as_string(std::string("hello")) << '\n'; // 1. koda girecek
	std::cout << as_string("hello") << '\n'; // 3. koda girecek bir hata olmayacak yine
}

Buradaki anafikir o kod instantiation da kullanılmıyorsa çok kısıtlı birinci fazda kontrol yapılacak.
Artık o kod seçilmediği için ikinci fazda kontrol yapılmayacak.

-------------------------------------------------------------------------------------------

ÖR:

ÇOK ÖNEMLİ ÖRNEK !!!!!!

COMPILE TIME IF ILE RUNTIME IF BIRARADA OLABILIR
FAZ1 FAZ2 OLAYI

#include <type_traits>

template <typename T>
void func(T tx)
{
	if constexpr (std::is_integral_v<T>) // Tamsayı türü ise aşağdaki if kodu yürütülecek
	{
		if (tx != 0) {  // kodda hata olabilir sadece anlaşılması için yazıldı
			func(tx--);
			//...
		}
	}
	else // ÇOK ÖNEMLİ BURASI !!!!! VISUAL C++ IN BURADA HATASI VAR
	{	 // STATIC ASSERT IF CONSTEXPR NIN DOĞRU KISMINDA OLSADA OLMASADA DERLENIYOR
		 // BURADA VISUAL STUDIO YANLIŞ KOD ÜRETIYOR. KODU DERLEMİYOR

		//undeclared_f(); //SENTAKS HATASI. BİR ISIM KULLANILMIŞSA FAKAT BU ISIM HERHANGIBIR ŞEKILDE
						 // TEMPLATE PARAMETRESINE VEYA ARGÜMANINA BAĞLI DEĞILSE,ELSE KISMINDA DA OLSA
						 // SENTAKS HATASI. 1. FAZDA YAPILAN KONTROL.YANI DAHA ÖNCE YAPILAN BIR KONTROL
						 // FUNCTION ÇAĞRILMASADA SENTAKS KONTROLÜ YAPILIR BURADA.BU İSİM ARANIP BULUNAMADI DİYECEK

		undeclared(tx);   //HERHANGİBİR SENTAKS HATASI YOK ÇÜNKÜ BU İSİM FUNC PARAMETRESİ İLE İLİŞKİLİ KULLANILMIŞ.
								  

		//static_assert(false, "not integral type"); //syntax error.Burası compile time ife bağlı değil yani
	}
}

int main()
{
	func(2);
	//func(2.5);
}

YANI STATIC ASSERT COMPILE TIME IF E BAĞLI DEĞIL. HERTÜRLÜ STATIC ASSERT DERLENIR.
VISUAL STUDIODA DERLENMİYOR. !!!!!!!!!! BU HATA BİLDİRİLMİŞ DÜZELTMEMİŞLER !!!!!!!!!!!

1. FAZDA(COMPİLE TİMEDA) YAPILANLAR = STATIC ASSERT KONTROLÜ ALWAS EVALUATED VE TEMPLATE PARAMETRESİNE BAĞLI OLMAYAN İSİMLER 1. FAZDA İSİM ARAMAYA TABİ
2. TEMPLATE PARAMETRESİNE BAĞLI İSİMLER 1. FAZDA KONTROLE TABİ DEĞİLLER VE HERHANGİ BİR ŞEKİLDE SENTAKS HATASI OLUŞTURMUYORLAR.

-------------------------------------------------------------------------------------------

ÖR:

#include <type_traits>

template <typename T>
void func(T& tx)
{
	if (tx > 0) { // tx > 0 ise aşağıda bir kod seçilimi yapılacak ++tx mi --tx mi olacağı t türünün
				  // tamsayı türü olup olmamasına bağlı.

		if constexpr (std::is_integral_v<T>) {
			++tx;
		}
		else {
			--tx;
		}
	}
}

#include <iostream>

int main()
{
	int ival = 5;
	double dval = 2.5;
	long lval = -4;

	func(ival); // ival 6 oldu
	func(dval); // 1.5
	func(lval); // if in doğru kısmına hiç girmeyecek

	std::cout << "ival = " << ival << "\n"; // 6
	std::cout << "dval = " << dval << "\n"; // 1.5
	std::cout << "lval = " << lval << "\n"; // -4 kalacak.
}

-------------------------------------------------------------------------------------------

constexpr auto func()
{
	if constexpr (sizeof(int) > 4u) {
		return 1;
	}
	else {
		return 0.5;
	}
}

int main()
{
	constexpr auto val = func(); // bizim derleyicide bu yanlış. func return değeri 0.5
}								 // doğru olsadı 1 dönecekti.

-------------------------------------------------------------------------------------------

BURASI ÇOK ÖNEMLİ !!!!!!!!!!!

#include <iterator>
#include <iostream>
#include <vector>
#include <list>


ADVANCE, ADVANCE_IMPL NIN ALTINDA OLDUĞU HALDE ÇALIŞIYOR.
BU INSTANTIATION ILE ALAKALI. DERLEYICI BURADA 1. FAZ KONTROLÜNDE ADVANCE IMPL ISMINI ARAMIYOR
ÖNEMLI OLAN INSTANTIATE EDILIDĞI NOKTADA GÖRÜLMESI.
ADVANCE ÇAĞRI YAPILDIĞINDA DERLEYİCİ ADVANCE_IMPL TEMPLATEİNİ INSTANTIATE EDECEK.

44:00 ITERATORLER ARASI KALITIM ILIŞKISINI DINLE
INSTANTIATION KONUSU DA İŞLENECEK.

//implementation for random access iterators
template <typename Iter, typename Dist>
void Advance_impl(Iter& pos, Dist n, std::random_access_iterator_tag)
{
	//std::cout << "random access iterator\n";
	pos += n;
}

//implementation for random bidirectional iterators
template <typename Iter, typename Dist>
void Advance_impl(Iter& pos, Dist n, std::bidirectional_iterator_tag)
{
	std::cout << "bidirectional iterator\n";
	if (n >= 0) {
		while (n--)
			++pos;
	}
	else {
		while (n--)
			--pos;
	}
}

//implementation for input iterators
template <typename Iter, typename Dist>
void Advance_impl(Iter& pos, Dist n, std::input_iterator_tag)
{
	//std::cout << "input iterator\n";

	while (n--) {
		++pos;
	}
}

template <typename Iter, typename Dist>
void Advance(Iter& pos, Dist n)
{
	using Cat = typename std::iterator_traits<Iter>::iterator_category; //okumayı kolaylaştırıyor
	Advance_impl(pos, n, Cat{});
}

int main()
{
	using namespace std;
	vector<int> ivec{ 1, 2, 3 ,4, 5 };
	auto veciter = ivec.begin();
	Advance(veciter, 3);
	std::cout << *veciter << "\n";

	list<int> ilist{ 1, 2, 3, 4, 5 };
	auto listiter = ilist.begin();
	Advance(listiter, 4);
	std::cout << *listiter << "\n";
}

-------------------------------------------------------------------------------------------

YUKARIDAKİ ÖRNEĞİ GÖRME SEBEBİMİZ AŞAĞIDAKİ KOD
AYNISINI IF CONSTEXPR İLE YAPIYORUZ VE DAHA KOLAY OLUYOR.

#include <iterator>
#include <type_traits>

template <typename Iter, typename Dist>
void advance(Iter& pos, Dist n)
{
	using cat = typename std::iterator_traits<Iter>::iterator_category;

	if constexpr (std::is_same_v<cat, std::random_access_iterator_tag>) {
		pos += n;
	}
	else if constexpr (std::is_same_v<cat, std::bidirectional_iterator_tag>) {
		if (n >= 0) {
			while (n--)
				++pos;
		}
		else {
			while (n++) {
				--pos;
			}
		}
	}
	else { //input iterator tag
		while (n--)
			++pos;
	}
}


-------------------------------------------------------------------------------------------

ÖR:

struct Nec {

};

int main()
{
	Nec nec;

	if constexpr (sizeof(int) > 4) {
		++nec;  //syntax error
	}
	else {
		--nec;   //syntax error
	}
}


Template olmayan bir kodda if constexpr kullanılabilir.
Bu durumda herhangibir şekilde ortada template yoksa if in doğru kısmıda yanlış kısmıda derlenecek.
Yani bu template e bağlı olsaydı, ++nec instantiate edilmeseydi bu kod sentaks hatası olmayacaktı.
Şimdi ister doğru ister yanlış ikisindede bu kontrol yapılıyor çünkü ortada template kod yok.
Bu durumda iki kodunda geçerli olması gerekiyor.

-------------------------------------------------------------------------------------------

BIRAZ KAFA KARIŞTIRICI OLABILIR ILERIDE ÖĞRENECEĞIMIZ TEKNIK VAR

class Nec
{
	int ival;
	double dval;
	std::string s;
};

template<size_t n>
auto& get(Nec& nec);

template<> auto& get<0>(Nec& s) { return s.ival; } // bunları constexpr if ilede yazabiliriaz.Önemli olan çağrıların geçerli olması
template<> auto& get<1>(Nec& s) { return s.dval; }	// o değerleri döndürmesi
template<> auto& get<2>(Nec& s) { return s.s; }

Nec foo()
{
	return { 12, 4.5, "salih" };
}

#include <iostream>

int main()
{
	auto[i,d,name] = foo();

	std::cout < i << " " << d << " " << name << "\n";
}

-------------------------------------------------------------------------------------------

AŞAĞIDAKI ŞEKİLDE IF COSNTEXPR ILEDE YAPILABILIR
3-4 AYRI TEMPLATE YAZACAĞIMA TEK BIR TEMPLATE ILE HALLEDIYORUM

class Nec
{
	int ival;
	double dval;
	std::string s;
};

template<size_t n>
auto& get(Nec& nec)
{
	if constexpr(n == 0)
		return nec.ival;
	else if constexpr(n == 1)
		return nec.dval;
	else
		return nec.s;

}

Nec foo()
{
	return {12, 4.5, "salih"};	
}

...
..

-------------------------------------------------------------------------------------------

template<int N>
constexpr int fibonacci() { return fibonacci<N - 1>() + fibonacci<N - 2>(); }

template<>
constexpr int fibonacci<1>() { return 1; }

template<>
constexpr int fibonacci<0>() { return 0; }


template <int N>
constexpr int fibo()
{
	if constexpr(N >= 2)
		return fibo<N - 1>() + fibo<N - 2>();
	else
		return N;
}

IF CONSTEXPR İLE 3 TANE TEMPLATE YERİNE 1 TANE İLE İŞ HALLEDİLİYTOR.
EXPLICIT SPECIALIZATIONDA YAPILMIYOR.

int main()
{
	constexpr auto x = fibonacci<8>();
	
	constexpr auto x = fibo<8>();
}

-------------------------------------------------------------------------------------------

auto func1(int x)
{
	if(x > 10)
		return 1;
					
	return 2; // Hidden / redundant else burası. else olmasada olsada kodun anlamı değişmiyo yani.
}

auto func2(int x)
{
	if constexpr (sizeof(int) > 4) // constexpr if te durum böyle değil. Bu koşul compile timeda bu koşul
	{								// kontrol edilecek.ve compile timeda derleyici ya instantiationda kullanacak yada
									// kullanmayacak. Kullanmazsa blokun bittiği yerden devam edeceği için bu sefer funcın return
									// değeri void olacak
		return 1;
	}
}

int main()
{
	auto x = func2(); // func2 de sizeof > 2 dene birde sizeof > 4 dene farka bak
						// 4 yazınca sentaks hatası oldu çünkü func return değeri void oldu.
}

-------------------------------------------------------------------------------------------

GÜZEL ÖRNEK

#include <iostream>
#include <memory>

template <typename T>
auto getval(T t)
{
	if constexpr (std::is_pointer_v<T>)
		return *t;
	else
		return t;
}

int main()
{
	int ival{ 87 };
	double dval{ 4.5 };
	int* iptr{ &ival };
	double* dptr{ &dval };

	std::cout << getval(ival) << "\n";
	std::cout << getval(iptr) << "\n";
	std::cout << getval(dval) << "\n";
	std::cout << getval(dptr) << "\n";
}

ifconstexpr ile compile timeda kod seçiliyor burada.

-------------------------------------------------------------------------------------------

Templateler ile ilgili önemli konu başlıkları
İsim arama konusu, ADL de dahil.Dependent name ve nondependent name kavramı.
Instantiation aşaması ve point of instantiation kavramı

NOT:
conditional<true,int,double>::type --> int verir
conditional<false,int,double>::type --> double verir

template <typename T>
void func(T x)
{
	std::conditional_t<std::is_integral_v<T>, int, double> y{}; // doğruysa int türü yanlışsa double türü.
	std::cout << typeid(y).name()<<"\n";
}


int main()
{
	func(12); // int
	func(23.4); // double
	func(2L); // double
}


CONDITIONAL YAZALIM

template <bool b,typename T, typename U>
struct Conditional
{
	using type = U;
}

template <typename T, typename U>
struct Conditional<true, T, U>
{
	using type = T;	
}

template <bool b,typename T,typename U>
using Conditional_t = Conditional<b, T, U>::type;

int main()
{
	Conditional<true,int,double>::type x; // x in türü int
	Conditional<false,int,double>::type x; // x in türü double

	Alias template te kullanılabilir.
	Conditional_t<true,int,double> x; // x in türü int

}

==========================================================================================================================================================================================
==========================================================================================================================================================================================
==========================================================================================================================================================================================
==========================================================================================================================================================================================

KONULAR
VARIADIC FUNC ŞABLONLARI
FOLD EXPRESSIONS
CTAD
DEDUCTION GUIDES

==========================================================================================================================================================================================
==========================================================================================================================================================================================
==========================================================================================================================================================================================

VARIADIC FUNC TEMPLATES
Öyle bir func olsunki buna istediğimiz kadar argüman gönderebilelim. Özellikle giriş çıkış işlemlerinde bu çok tipik bir ihtiyaç.
4 değer gönderirsek onun en küçüğü, 5 gönderirsek onun en küçüğü gönderilecek örneğin min func yada print func

C de bu ihtiyaç alt seviyeli yöntemle yapılıyor. C deki variadic funclar.C deki Yöntemlerin dezavantajları
1- Type safe değil.
2- İstediğim sayıda argüman gönderebiliyorsam bile kaç argüman gönderdiğimi göstermeliyim.

Çok kısaca C deki variadic funclara baktık.
Buna birşey yazmayacağım.

C++taki variadic parametreler.

template <typename T, typename ...Args> // Burada nontype ve type parametre için kullanılabilir bunlar. Template parameter pack burası
void func(Ts ...args) // param packte 3 parametre varsa, bu functionun 3 tane parametresi olacak ve 1. parametresi param packteki
					  // 1. türden, 2. parametresi 2. türden, 3. parametresi 3. türden


template <typename T, typename U, typename K>
void func(T p1, U p2,K p3)
{

}

int main()
{
	func(1,2,3); // bu çağrı olsaydı T U ve K nın çıkarımını int olarak yapacaktı ve derleyicinin yazdığı func ın 3 tane parametre dğeişkeni olacaktı
				 // bunlar int türden parametreler.

	func(); // buda geçerli.Template param packte ve func param packte 0 tane parametre var.
}

----------------------------------------------------------------------------------------------------------------------------------------------------

template <typename ...Ts>
void func(Ts&...args)
{
	
}

int main()
{
	int x{};
	double d{};

	func(x,d); // Template param pack teki türler int ve double. Func parametre değişkenleri int&,double& olacak. Const koysaydık const int & ....
}

----------------------------------------------------------------------------------------------------------------------------------------------------

FORWARDING REFERANS

template <typename ...Ts>
void func(Ts&&...args)
{
	
}

int main()
{
	int x{};
	double d{};

	func(x,d); // Template param pack teki türler int ve double. Func parametre değişkenleri int&,double& olacak. Const koysaydık const int & ....
}

----------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------

SIZEOF...

Compile timeda variadic parametreye sahip func templateinin, template param packteki yada func param packteki öğe sayisini öğrenmek için kullanilir
Gerek func param pack gerek template param pack sizeof operatorunun operandı yapılaibilir.

template <typename ...Ts>
void func(Ts&&...args)
{
	constrexpr auto n1 = sizeof...(args);
	constrexpr auto n2 = sizeof...(ts);
	
	std::cout << "n1 = " << n1 << "\n";
	std::cout << "n2 = " << n2 << "\n";

}

int main()
{
	int x{};
	double d{};

	func(x,d); // 2 çıktı
	func(x,d,45); // 3 çıktı
	func(x,d,45,'T'); // 4 çıktı

}

----------------------------------------------------------------------------------------------------------------------------------------------------

Func parametre pakedi tek başına olmak zorunda değil.
başka parametrelerde olabilir başka template parametrelerine bağlıda olabilir.

template <typename T, typename ...Args>
void func(T x, Args ...args);

int main()
{
	func(1,2.3,5, 8L); // T int olacak ama 2.3.ve 4. argüman parametre pakedine girecek.
}

----------------------------------------------------------------------------------------------------------------------------------------------------

template <typename ...Args>
class TypeTeller;

template <typename ...Args>
void foo(Args ...)
{
	TypeTeller<Args...>x;	
}

template <typename ...Args>
void func(Args ...args)
{
	foo(args...); // 
}

DERLEYİCİ FUNCI AŞAĞIDAKİNE ÇEVİRİYOR
void func(T1 param1, T2 param2, T3 param3)
{
	foo(param1, param2, param3);
	Bundan istifade etmemizin tek yolu bu değil. 10 a yakın extension örüntüsü var
	dil tarafından garanti altına alınmış.

	--------------------------------------------------------------------------------------------

	//Eğer func aşağıdaki olsaydı
	foo(&args...);
	//Derleyici aşağıdakine açacaktı
	foo(&param1, &param2, &param3);
	 
	--------------------------------------------------------------------------------------------
	
	//Eğer func aşağıdaki gibi olsaydı
	foo(f(args)...);
	//aşağıdakine dönüşecekti
	foo(f(param1), f(param2),f(param3));

	--------------------------------------------------------------------------------------------

	PACK EXPANSION HER CONTEXTE KULLANILAMIYOR.BELİRLİ KOŞULLARIN SAĞLANILMASI GEREKİYOR
	HERYERDE KULLANILAMIYOR.

	std::tuple<Args>(args)...;

	tuple<param1,param2,param2>(arg1,arg2,arg3);

}

----------------------------------------------------------------------------------------------------------------------------------------------------

template <typename ...Args>
class TypeTeller;

template <typename ...Args>
void foo(Args...)
{
	TypeTeller<Args...>x;
}	// Buraya kadar olan kod typeteller ile türü anlama kodu

template <typename ...Args>
void func(Args ...args)
{
	foo(args...);
}

int main()
{
	func(1,4.2,"mehmet"); // int double const char * verir.

	----------------------------------------------------------

	foo(&args...); //olsaydı funcin içindeki çağrı elemanların adresiyle çağrılacaktı
	
	func(1,4.2,"mehmet"); //int *, double *, const char **
}

----------------------------------------------------------------------------------------------

f(&args...); // expands to f(&E1, &E2, &E3)
f(n, ++args...); // expands to f(n, ++E1, ++E2, ++E3);
f(++args..., n); // expands to f(++E1, ++E2, ++E3, n);
f(const_cast<const Args*>(&args)...);
// f(const_cast<const E1*>(&X1), const_cast<const E2*>(&X2), const_cast<const E3*>(&X3))
f(h(args...) + args...); // expands to
// f(h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3)

----------------------------------------------------------------------------------------------

template <typename T, typename ...Args>
std::unique_ptr<T> MakeUnique(Args&& ...args)
{
	std::unique_ptr<T> {new T (std::forward<Args>(args)...)};
	// Dinamik oluşturulan T türünden nesnesinin constructoruna forward çağrılarının return 
	// değerlerini gönderdik.Perfect forward ediyor yani
}

----------------------------------------------------------------------------------------------

template <typename ...Types>
void func(Types ...args)
{
	std::tuple<Types...>x;

	std::cout << typeid(x).name() << "\n";
}

int main()
{
	func(1,2.3,4L); //tuple<int,double,long>
}

----------------------------------------------------------------------------------------------

template <typename ...Types>
void func(Types ...args)
{
	std::tuple<Types ...>x(args...);

	std::cout << typeid(x).name() << "\n";
}

int main()
{
	func(1,2.3,4L); //tuple<int, double,long>x(p1,p2,p3);
}

----------------------------------------------------------------------------------------------


template <typename ...Types>
void func(Types ...args)
{
	std::tuple<Types ...>x(std::forward<Types>(args...);

	std::cout << typeid(x).name() << "\n";
}

int main()
{
	func(1,2.3,4L); //Perfect forward edilmiş olacak.
}

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

VARIADIC FUNC ŞABLONUNDA KULLANILAN 3 GENEL TEKNİK VAR

1- COMPILE TIME RECURSIVITY - EKSILTME TEKNİĞİ
2- C++17 İLE GELEN FOLD EXPRESSIONS
3- COMMA SEPARATED LIST

----------------------------------------------------------------------------------------------

///COMPILE TIME RECURSIVE PACK EXPANSION. TEMEL C++ TA GÖRDÜK AÇIKLAMA YAPMADIM

#include <iostream>

template <typename T>
void print(const T& r)
{
	std::cout << r << "\n";
}

template <typename T, typename ...Ts>
void print(const T& r, const Ts& ...args)
{
	std::cout 
	print(r); // std::cout << val << "\n"; buda yazılabilirdi. İlk örnekte hatta bu halini yazmış hoca.
	std::cout << __FUNCSIG__<<"\n"; // her seferinde farklı func olduğunu ispatlamak için signature yazıldı
	print(args...);
}

int main()
{
	int x = 10;
	double d = 23.5;

	print(x, d, 9L, "necati");
}

NOT: Variadic olmayan parametreli func, variadic olana göre önceliklidir.

MOLA

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------


#include <ostream>

template<typename T>
std::ostream& print(std::ostream& os, const T& t)
{
	return os << t;		// no separator after the last element in the pack
}

// this version of print will be called for all but the last element in the pack
template <typename T, typename... Args>
std::ostream& print(std::ostream& os, const T& t, const Args& ...rest)
{
	os << t << ", "; // print the first argument
	return print(os, rest...); // recursive call; print the other arguments
}

#include <iostream>

int main()
{
	print(std::cout, 45, "ali", 3.87, 'A');
}

BUNLAR ZAHMETLİ AMA BAŞKA YOLLARDA VAR.

------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------

VİRGÜL(,) OPERATORÜ HEM C DE HEMDE C++ TA 2 FARKLI AMAÇLA KUKKANILIYOR.
1 - VİRGÜLLERLE AYRILMIŞ LİSTENİN VİRGÜLÜ YANİ SEPERATOR OLAN VİRGÜL
2 - OPERATOR VİRGÜL

void func(int x, int y, int z); // bunlar seperator
func(3,4,5); //seperator

int a[] = {1,2,3,4,5}; //Seperator
int a[] = {(1,2,3,4,5)}; // Burada Operator Virgül oldu

func(x,y); // seperator
func((x,y)); // operator
, aynı zamanda sequence point oluşturuyor.

expr1;
expr2;
expr3;

expr1,expr2,expr3; // anlamsal farklılık yok.

a[2,4] böyle kullanılamıyor eskiden varmış.

int main()
{

	int x = 10, y = 24;

	auto a = (x,y);
	std::cout << "a = " << a << "\n"; // 24 oldu a

	a=(y,x);
	std::cout << "a = " << a << "\n"; // 10 oldu şimdide a
}


(x,y) = 9; // C de x ve y lvalue expr olsa bile atanamaz. C++ ta bu kod legal

------------------------------------------------------------------------------------------------

void f1() { std::cout << "f1()\n";}
void f2() { std::cout << "f2()\n";}
void f3() { std::cout << "f3()\n";}

int main()
{
	f1(),0 // ifadenin türü = int, ifadenin değeri = 0. önce f1 çalışır burada hatta.

	int a[] = {(f1(),0), (f2(),0),(f3(),0)};
}

------------------------------------------------------------------------------------------------

template <typename ...Types>
void Print(const Types& ...args)
{
	int a[] = {((std::cout << args << " "), 0)...}; //aslında bu int diziye virgüllerle ayrılmış liste
}													// cput ilede ekrana yazdırma yapıyor.

ORDER OF EVALUATION KONUSUNA BAKMAK GEREKEBİLİR YADA HOCA ANLATACAK

int main()
{
	print(1,2.3.6L);
}

Bunuda hoca tercih etmiyor çünkü dizi tanımlanıyor.

------------------------------------------------------------------------------------------------

BUNDAN DAHA İYİSİ İNİTİALİZER LİST KULLANMAK

template <typename ...Types>
void Print(const Types& ...args)
{
	(void)std::initializer_list<int>{((std::cout << args << " "),0)...};
}

int main()
{
	Print(1,2.3,6L,"mehmet");
}

------------------------------------------------------------------------------------------------

ÖNEMLİ ÖRNEK

template<typename T>
T summer(T v)
{
	return v;
}

template<typename T, typename... Args>
T summer(const T& first, const Args& ... args)
{
	return first + summer(args...); // eksildi eksildi. en son pakette 1 tane kalınca diğer overload yani yukarıdaki seçildi.
}

#include <iostream>
#include <string>

int main()
{
	std::cout << summer(10, 20, 30, 80, 17) << '\n';
	std::string s1 = "nec", s2 = "ati ", s3 = "Er";
	std::cout << summer(s1, s2, s3, "gin") << '\n';
}

------------------------------------------------------------------------------------------------

COMPILE TIME IF
BURADA BASE CASE İÇİN AYRI FUNC YOK

#include <ostream>

template <typename T, typename... Args>
std::ostream& print(std::ostream& os, const T& t, const Args& ...rest)
{
	os << t;

	if constexpr (sizeof...(rest) > 0) // paketteki parametre sayısı 0 değilse buraya girecek
		os << ", ";
	else
		os << "\n";

	if constexpr (sizeof...(rest) != 0) // paketteki öğe sayısı 0 sa.
		print(os, rest...);

	return os;
}

#include <iostream>

int main()
{
	print(std::cout, 45, "ali", 3.87, 'A');
}

------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------

FOLD EXPRESSIONS
C++17 İLE BİRLİKTE VARIADIC FUNCLARDA KULLANABİLECEĞİMİZ BİR ARAÇ

Dilin sentaksına göre garanti altına alınmış bazı ifadeler var. Bunlar fold ifadeler.

Unary left fold
Unary right fold
binary left fold
binary right fold

template<typename ...Args>
auto Sum(const Args& ...pack)
{
	return (... + pack);  // Derleyici bu ifadeyi tüm parametreleri toplama operatörünün operandı yapıyor.
	return (((p1 + p2) + p3) + p4);
	
	------------------------------------------------------------------------------------------------------

	return (pack + ...  ); 
	return(p1 + (p2 + (p3 + p4)));

}

------------------------------------------------------------------------------------------------

template <typename ...Args>
auto Sum_r(const Args& ...pack)
{
	return (pack + ...);
}

template <typename ...Args>
auto Sum_l(const Args& ...pack)
{
	return (... + pack );
}

int main()
{
	string s1{"ali"}, s2{"veli"},s3{"gul"};

	std::cout << Sum_l(s1,s2,s3,"necati","ergin");
	
	std::cout << Sum_r(s1,s2,s3,"necati","ergin"); //Sentaks hatası çünkü "necati" + "ergin" 
}													// iki pointer toplaması sentaks hatası


*/
