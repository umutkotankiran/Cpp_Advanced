/*

TEMPLATES
---------

Derleyiciye kod yazdırma amaçlı kullanılan araç. C++ ın en temel araçlarından biri.
Standart library nin neredeyse tamamı template.

Template sadece C++ ta yok ama generic programmingin kendini en olgun noktaya
getirdiği dil C++. Metacode derleyiciye kod yazdırmak için kullanılan kod.

Template Türleri
----------------
function template
class template 
alias template - Modern C++ ta geldi
variable template - Modern C++ ta geldi

Template ten üretilen gerçek kod, o template in specializationıdır.
Functionun şablonundan derleyicinin bir kod yazma sürecinede o templatein
instantiate edilmesi deniyor.Bu sürecede instantiation deniyor.

Using template haline getirilibeliyor.Bu yüzden using dile eklendi.
typedef ile bildirilen türeş isimleri template haline getirilemiyor.

Conceptleri de anlatacak hoca.

Container adapters : Containerı, containment compositionu yoluyla eleman olarak alıyor ve
interfaceini temsil ettikleri abstact data type için implemente ediyorlar


---------------------------------------------------------------------------------------------------------

SINIF ŞABLONLARI
----------------

TEMEL C++ TA DETAYLI GÖRÜLDÜ. EĞER GÖRÜLMEYEN NOKTALAR VARSA GÖRÜLMEDİ YAZACAĞIM.

SEQUENCE CONTAINERS
	std::vector			
	std::deque			
	std::list			
	std::forward_list	
	std::string			
	std::array	
	C Arrays

ASSOCIATIVE CONTAINERS
	set
	multiset
	map
	multimap

UNORDERED ASSOCIATIVE CONTAINERS
	unordered set
	unordered multiset
	unordered map
	unordered multimap

CONTAINER ADAPTERS
	Stack
	Queue
	Priority Queue

SMART POINTERS
	unique ptr
	shared ptr
	weak ptr
	Boostta çok daha fazlası var tabi.

Hash sınıf şablonları var, iteratorler var ... birsürü var.

BIRDE METAFUNCTION OLARAK KULLANILAN SINIF ŞABLONLARI VAR
type traits, bunların amacı metafunction olarak görev yapmak.Yani compile time programlamada bizim için veri üretiyor.

GERÇEK FUNCTION VAR, BIRDE METAFUNCTION VAR. METAFUNCTION BIZE COMPILE TIMEDA DEĞER ÜRETIYOR. NORMAL FUNCTION DATA ISTERKEN, 
METAFUNCTION BIZDEN TEMPLATE ARGÜMANI ISTIYOR.METAFUNCTIONLAR GELENEKSEL OLARAK EĞER COMPILE TIMEDA DEĞER HESAPLAYACAKSA BUNU
STATIC CONSTEXPR VALUE ELEMANI. BIR TÜR HESAPLANACAKSA ONUDA TYPE ELEMANI TUTUYOR. IS_LVALUE REFERENCE MESELA METAFUNCTION !!!!!!!!!!!!!!!!!!!!!!

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	using namespace std;

	BU COMPILE TIMEDA TÜR SORGUSU YAPIYOR.

	is_lvalue_reference<int &>::value // Bu tür bir sol taraf referansıysa value elemanı compile timeda true döner.Aksi halde false.

	---------------------------------------------------------------------------------------------------------------------------------------------------------------

	TÜR VEREN FUNCLARDA VAR

	remove_reference int& verirsek int verir, myclass&& verirsek myclass verir. Bunlar hep compile timea yönelik.

	remove_reference<int&>::type --> int verir.

}

INTEGRAL CONSTANT DİYE BİR TEMPLATE VAR.BUNU STANDART LIBRARY VERIYOR. BUNDAN HAREKETLE DİĞER METAFUNCLARI OLUŞTURUYORUZ.TEMEL C++ TA GÖRÜLDÜ

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TYPENAME KONULMASI GEREKEN YERLER VAR. C++ 20 İLE BİRAZ GEVŞETİLDİ BU KURALLAR

ALİAS TEMPLATE ÖRNEK
template <typename T>
using RemoveReference_t = typename std::remove_reference<T>::type;

VARIABLE TEMPLATE ÖRNEK
template<typename T>
constexpr bool IsRvalueReference_v = std::is_rvalue_reference<T>::value;

Bunda inline da olabilir bu bize bağlı.

Aynı kaynak dosyada aynı 2 tanım sentaks hatası, farklı kaynak dosyalarda aynı varlığın birden fazla tanımı doğrudan undefined behavior.
Eğer inline keywordünü yazarsak inlşine statüsüne girer yani link aşamasında linker bundan sadece 1 tane görecek ve ODR bozulmayacak

inline void func(int)
{
	
}

İnline koymasakta derleyici inline expansion yapabilirse yapar zaten.İnline ın asıl işlevi bu değil artık.
Token by token tanım aynıysa ODR bozulmaz.

Function şablonları sınıf şablonları class definitionlar bunlar header file a konuyor ve ODR ihlal edilmemiş olmuyor.
C++17 ile değişkenlerede uyguladılar.

inline int x = 23; 
İnline tanımlanan değişkenler, token by token aynı olmak koşuluyla birden fazla kod dosyasında bu header file include edilmiş olsa bile,
ortada birden fazla tanım olmasına rağmen bu tanımlar birbirine eş olduğunda link zamanı için baktığımızda sadece 1 değişken olacak.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATE PARAMETERS
Func parameter ile template parameters karıştırılmamalı.< > içerisine yazılan parametrelere template parameters deniyor.

template <typename T> // T template tür parameter
void func(T x, T y) // x ve y function parametre. Bunlara call parameterde deniyor.
{
	
}


TÜRLER
------
Type
Nontype
Template Template

NOT : Bunların hepsi variadic olabilir.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TYPE BIR TÜRE KARŞILIK GELIYOR.

template <class T> // burada ilk başlarda class vardı.
void func(T x)
{
	
}

template <typename T> // Artık typename kullanılıyor
void func(T x)
{
	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

NONTYPE

Tür yerine geçmeyen yani bir sabitten bahsediliyor.

template <int x>

ÖR:
template<typename T, size_t n>
struct Array
{
	T a[n];
}

template <size_t n>
struct Bitset
{
	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

NONTYPE IN HANGI TÜRLERDEN OLABILECEĞI YILLAR GEÇTIKÇE GENIŞLETILDI.

İlk standartlarda 
- Tamsayı türleri kullanılabilir.
- Pointer türleri kullanılabilirdi.
- Function Pointer türleri kullanılabilirdi

C++20
- Nontype parametre reel sayı olabilir.

template <double x>
class Myclass{

};

int main()
{
	Myclass<2.3> x; // C++ 20 ile geldi.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template < > // açısal parantez içerisine constexpr ctoru olan nesne türleri konabiliyor. 
class Myclass
{

};

template<auto n> // Nontype ama Generalized. C++14 olmalı. Ne kullanırsak nontype parametrenin türü
class Myclass{

};

int main()
{
	Myclass<10> ; // template parametre int olarak alınacak
	Myclass<20U> ; // template parametre unsigned int olarak alınacak
	Myclass<2.4> ; // template parametre double olarak alınacak
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

INTEGRAL CONSTANT
-----------------
Metafunction oluşturma konusunda asıl desteği veren taban sınıf.

template<class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant; // using injected-class-name
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } // C++14
};

true_type	std::integral_constant<bool, true>
false_type	std::integral_constant<bool, false>

value_type		T
type			std::integral_constant<T,v>

constexpr T value       static constant of type T with value v

operator value_type	returns the wrapped value
 
operator()		returns the wrapped value
 

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATE TEMPLATE PARAMETER. TEMEL C++ TA GÖRÜLMEDİ

template<template<typename, typename> class X>  // Bu template, bu partametreye karşı argüman olarak gelecek tür, 2 tane tür parametresine sahip bir template olacak.
BUrada class keyword zorunluydu typename sentaks hatasıydı. Artık class yerine typename kullanılabilir halde artık.

template<template<typename, typename> typename X> // typename geldi.


template <typename T>
class Myclass{
	//..
};

template <typename T, template<typename> typename X>
void func(T x, X<T>)
{
	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

YUKARIDAKİ MYCLASSI KULLANACAĞIZ BU ÖRNEKTE

template <template<typename> typename X>
class Nec{
public:
	Nec()
	{
		std::cout << typeid(X<int>).name()<<"\n";
	}

};

int main()
{
	Nec<Myclass>x; // Burada myclass ta bir template. Nec in myclass template açılımı diye okunabilir mesela
}

ÇIKTI
-----
class Myclass<int>  

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR: VECTOR ALAN TEMPLATE GİBİ DÜŞÜNÜLEBİLİR

template<typename T, typename A, template<typename,typename> class Con> // DİKKAT!!! class yerine tytpenamede kullanabilirdik.
void func(Con<T,A>&x) //Vector gibi birincisi tür ikincisi allocator gibi düşübülebilir.
{
	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR: NONTYPE PARAMETRELİ TEMPLATE, TEMPLATE TÜR PARAMETRESİ OLARAK YAZILMIŞ

template<typename T, typename A, template<int> class Con> // bu template bir tane nontype parametreye sahip template istiyor
void func(Con<T,A>&x)
{
	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR: HEM NONTYPE HEMDE TYPE PARAMETRELİ TEMPLATE TEMPLATE PARAMETER

template<typename T, typename A, template<typename, int> class Con> // birinci parametresi tür, ikincisi nontype parameter.
void func(Con<T,A>&x)
{
	
}

Bunları kullanacağımız yerler sınıflı ama ileride öğreneceğimiz tekniklerde template template parametre yerini alacak.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Eğer ortada bir template varsa func şablonu yada sınıf şablonu, derleyici bu template ten kod yazabilmesi için bu template parametrelerine karşılık gelen türleri sabitleri
veya template leri compile timeda bilmesi gerekiyor ki ona göre yazsın.

Derleyici bunu nasıl bilecek ? 
Derleyicinin bu template te template parametreleri için bir substitution yapması gerekiyor.
Substitution, T yerine gerçek bir koyması anlamında. 

Buna yönelik 3 araç var.
1 - Deduction : Çıkarım. Derleyici template kullanıldığı contexe bakarak dilin kurallarını uygulayarak template parametresine karşılık gelen argümanın hangi sabit olduğunu anlamaya çalışacak.
		Template argümanlarının contexten çıkarım yoluyla elde edilmesi sürecine template argument deduction deniyor. C++17 de ciddi farklılık oldu. C++17 den önce bu sadece func 
		template için vardı, C++17 ile CTAD geldi.Class template argument deduction ile belli bir seviyede sınıflar içinde deduction imkanıda var.


int main()
{
	vector x = {1,2,3,4,5};
	vector y = {x,x}; // y nin türü vector<vector<int>>; Ctad var.

	optional x = 5; // CTAD. Constructora bakarak derleyici deduction yapıyor compiler burada.
			
	pair p1 = {23 ,45};
	pair p2 = {2.3f ,'t'}; //İkiside CTAD
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------
				
SADECE TYPE IÇIN DEĞIL NONTYPE IÇINDE GEÇERLI.
template<typename T, size_t n>
void func(Ta (&r)[n]);

int main()
{
	int a[10]{};
	func(a); //CTAD VAR YİNE
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

int foo(double)
{
	return 1;
}

template<typename T,typename U>
void func(T(*p)(U))
{
	std::cout << typeid(T).name() <<"\n";
	std::cout << typeid(U).name() <<"\n";
}

int main()
{
	func(foo); // decay olacak. Function to pointer conversion.return değeri int parametresi double olan func adresi olacak template te duduce edilecek.
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

MEMBER FUNCTION POINTER 
class Nec {
public:
	int foo(double) { return 1; }
};

template <typename C, typename T, typename U>
void func(T(C::*)(U))
{
	std::cout << typeid(C).name() << '\n';
	std::cout << typeid(T).name() << '\n';
	std::cout << typeid(U).name() << '\n';
}

int main()
{
	func(&Nec::foo);
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

2 - Explicit Template Argument : Derleyici sen çıkarım yapma ben template parametresi karşılık gelecek argümanın ne olacağını ben yazacağım.
								 
template<typename T, typename U>
void func(T,U)
{
}

int main()
{
	func<int, double>(12,2.4); // Burada çıkarım olmayacak.
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	make_unique<int>(12); //Neden explicit yazdık. Çünkü öyle senaryolar varki derleyici ilgili türü deduction yapma şansı yok.
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

Make Unique bakalım.T türü çıkarımı yapılamıyor bunda

template <typename T, typename ...Types>
std::unique_ptr Make_Unique(Types&&... args)
{
	return std::unique_ptr<T>{new T{std::forward<Types>(Args)...}}; 
}

------------------------------------------------------------------------------------------------------------------------------------------------------------

EXPLICIT TEMPLATE ARGUMENT KULLANDIĞIMIZDA BÜTÜM TEMPLATE PARAMETRELERI IÇIN ARGÜMAN VERMEK ZORUNDA DEĞILIZ

template<typename T, typename U>
void func(T x, U y)
{
			
}

int main()
{
	func<int>('A',4.5); // 1. template tür parametresinin int olduğunu biz belirtmişiz ama 2. temp arg. için biz söylemedik deduction yapacak.
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

NONTYPE PARAMETERDA OLABİLİR.
			
template<int x, int y>
void func()
{
	// değerler burada kullanılacak.
}

int main()
{
	func<5.10>(); //
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

3 - Default Template Argument
	
template<typename T, typename U = int>
class Myclass{

};

int main()
{
	Myclass<std::string, double> // T için string, U için double 
	Myclass<std::string> // T için string, U int
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

NOT : VARSAYILAN TEMPLATE ARGÜMANI MODERN C++ ÖNCESİNDE FUNC ŞABLONLARI İÇİN GEÇERLİ DEĞİLDİ.SADECE SINIF ŞABLONLARI VARSAYILAN ARGÜMAN ALABİLİRDİ.
C++11 ILE BIRLIKTE FUNC ŞABLONLARINDA KULLANILIYOR.

---------------------------------------------------------------------------------------------------------------------------------------------------------

SET : template<class Key,class Compare = std::less<Key>,class Allocator = std::allocator<Key>> class Set;

---------------------------------------------------------------------------------------------------------------------------------------------------------------

vector<int> ; // vector<int,std::allocator<int>> demek

---------------------------------------------------------------------------------------------------------------------------------------------------------------

set<int> ; // set<int, std::less<int>,allocator<int>> demek. Not: less functional header file içinde

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
struct DefaultDelete {

	void operator()(T* p)
	{
		delete p;
	}
};


template <typename T, typename D = DefaultDelete<T>>
class UniquePtr {
public:
	//...
	~UniquePtr()
	{
		//...
		D{}(mp);
					}
	private:
	T* mp;
};

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename Ch, typename Trait = std::char_traits<Ch>, typename Alloc = std::allocator<Ch>>
class BasicString {

};

using String = BasicString<char>; // Burada da typedef ismi.


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATE LER IÇIN DERLEYICI TIPIK OLARAK 2 FAZLI BIR KONTROL YAPIYOR.TEMEL C++ TA GÖRÜLDÜ BURASI

1. Faz
------
Ortada herhangibir kod yazma söz konusu yani instantiation yok, derleyici genel olarak sadece template kodu görüyor.Bu durumda derleyicinin yapacağı kontroller sınırlı
isim arama, küme parantezi veya normal parantez kapanmış mı gibi....
Eğer kullanılan bir isim template parametresine bağlı değilse yani dependent name değilse o ismin bulunması gerekir.
Ör:
template<typename>
void func(T x)
{
	x = y; // Bunu yazamayız mesela. X ismi biliniyor ama Y bilinmiyor.

	T::y = 5; // Hata veremez
}

Derleyici bu template ten henüz func kodu yazmıyor olmasına rağmen sentaks hatasını verecek.
Kullandığımız isim template parametresine bağlıysa derleyici hata verme şansına sahip değil.
İki örnekte yukarıda yazıldı

Template argument deductionda önemli noktaya bakalım.
Auto specifier ile yapılan tür çıkarımı arasında 1 istisna dışında bir fark yok.
Initializer list bu istisna.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

Buradaki 3 temel pattern
template<typename T>
void func(T x);

template<typename T>
void func(T& x);
	
template<typename T>
void func(T&& x);
	
---------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
void func(T x);


int foo(int);

int main()
{
	const int cx = 10;
	int y = 10;
	int& r = y;
	const int& cr = cx;

	func(12); //T is int
	func(y); //T is int
	func(r); //T is int
	func(cr); //T is int
	func("ali"); //const char *
	func(foo); // int(*)(int)
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T, int N>
void func(const T(&)[N])
{
	std::cout << typeid(T).name() << ' ' << N << '\n';
}

int main()
{
	int a[10]{};

	func(a);
	func("mert");
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void func(T& x);

int main()
{
	int x = 10;
	int& r{ x };
	const int cx = 23;
	const int& cr{ cx };

	//func(10); //gecersiz
	func(x); // T is int
	func(r); // T is int
	func(cx); // T is const int
	func(cr); // T is const int

	int a[3]{};
	func(a); / T is int[3]
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void func(T& x);

int main()
{
	int a[3]{};
	const int ca[] = { 1, 3, 5, 7, 9 };
	int* p{ a };
	const int* cp{ ca };

	func(a); // T is int[3]
	func(ca); // T is const int[3]
	func(p); // T is int *
	func(cp); // T is const int *
	func("deniz"); // T is const char[6]
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------
	
template <typename T>
class TypeTeller;


template <typename T>
void func(T&) {
	TypeTeller<T> x;
}

int foo(double);

int main()
{
	func(foo);  // T is int(double)
}
	
---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TypeTeller;

template <typename T> //forwarding referans ama içeride perfect forwarding olmak zorunda değil.
void func(T&&)
{
	TypeTeller<T> x;
}

int main()
{
	func(5); //T is int
	int ival{};
	func(ival); //T is int &
	const int cx{20};
	func(cx);  //T is const int &
	func("mert"); //T is const char(&)[5]
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
void func(T&& x,const std::vector<T> &ivec)
{
		
}

int main()
{
	using namespace std;

	vector<int> ivec(10);

	func(ivec[0],ivec); // birinciden T int& çıkar çünkü ivec[0] bir referans döndürür. İkinciden ise T int çıkar.Ambigiuty
}

type identity kullanarak tür çıkarımını engelleyebilirdik yada explicit olarak tür parametresini verirdik. 

---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void func(T x, std::add_lvalue_reference<T> y) //Birinci parametre için tür çıkarımı yapılacak, 2 parametre birinci parametre için tür çıkarımı yapıldığında oradan elde edilecek.
{
		
}
	
int main()
{
	
}



======================================================================================================================================================================================================
======================================================================================================================================================================================================
======================================================================================================================================================================================================
======================================================================================================================================================================================================


2. Faz
------

1 - Func şablonları overload edilebilir mi ? Evet
2 - Bir func şablonuyla aynı isimli func birarada bulunabilir mi ? Evet
3 - Eğer evet ise ikiside, func çağrısında resolution nasıl yapılacak?

KRİTİK YER !!!

void func(int);
void func(int); // redecleration

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void func(int); // 
int func(int); // Sentaks hatası

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GÜZEL SORU !!! GEÇERLİ Mİ ? HAYIR.SENTAKS HATASI. Bir func teplate ini 2 kere tanımlayamayız.
template<typename T>
void func(T x)
{

}
template<typename T>
void func(T x)
{

}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


template<typename T>
double func(T x)
{
	return 1.1;
}
template<typename T>
int func(T x)
{
	return 1;
}

GEÇERLİ
NEDEN GEÇERLİ ? 
FUNC TEMPLATELERDEKİ KURALLAR İLE GERÇEK FUNCLARDAKİ İMZA KURALLARI AYNI DEĞİL. !!!!!!!!!!!!!!
ŞABLONLARDA FUNC RETURN DEĞERİ İMZAYA DAHİL. !!!!!!!!!!!!

BU TEMPLATELERİN BİRARADA BULUNMASI BAŞKA BİRŞEY, BU FUNCA YAPILAN ÇAĞRIDA BURADA AMBIGIUTY OLUŞMASI FARKLI BİRŞEY.
BU FUNCA YAPILAN ÇAĞRIDAN BAHSETMİYORUZ. AYNI KAYNAK DOSYADA BULUNUP BUNLARI OVERLOAD EDEBİLMEKTEN BAHSEDİYORUZ. BURADAKİ OLAY İMZA FARKLILIĞI.
COMPILE EDİNCE GEÇERLİ

BU İKİ TEMPLATE İN BİRBİRİNİ OVERLOAD ETMESİ, BU FUNCTİONA YAPILAN ÇAĞRILARIN HEPSİNİN LEGAL OLMASI ANLAMINA GELMİYOR.

BU FUNCLARA ÇAĞRI YAPILIRSA AMBIGIUTY OLUR FUNC ÇAĞRISININ NASIL YAPILDIĞINA BAĞLI OLARAK.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T, typename U>
void func(T x, U y) //SIRAYA DİKKAT !!
{ 
	
}

template<typename T, typename U>
void func(U x, T y) //SIRAYA DİKKAT !!
{

}

GEÇERLİ YANİ OVERLOADING. SENTAKS HATASI DEĞİL.OVERLODİNG!!!! ÇÜNKÜ İMZA FARKLI YANİ SIRANIN TERS OLMASI TEMPLATELERDE İMZA FARKLILIĞI OLUŞTURUR.

AMBIGIUTY FUNC ÇAĞRISI İLE OLUR. BU FUNCA ÇAĞRI YAPTIĞIMIZDA AMBIGIUTY OLUR 
BURADA BAHSEDİLEN İSE BUNLARDAKİ İMZA FARKLILIĞI.ÇAĞRILIRSA NE OLUR O FARKLI BİR DURUM.


*/
