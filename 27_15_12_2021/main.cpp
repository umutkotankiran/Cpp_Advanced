/*

C++20 İLE GELEN YENİLİKLERDEN DEVAM
-----------------------------------
1. Aggregatelere ilişkin kurallarda değişiklik var.Geçmiş zamanlardaki kodları
derlerken dikkatli olmalıyız. Eskiden aggregate olan bazı durumlar şimdi o statüden
çıktı. Eğer Ctor User Declared olursa artık aggregate değil. Aggreage olmamasının sonucu
aggregate init yapılamıyor olması artık.

struct Nec{
	int x,y;
	Nec() = default; // deleted yada user declared olsada yine aynı durum
	
	int a = 23; // Default init yapılması Aggreage initi bozmuyor
};

int main()
{
	Nec n1{1,2}; // GEÇERLI C++17 
	
	Nec n1{1,2}; // HATA C++20 
}

BİR ÖNCEKİ DERSTE BU MADDE YAZILDI AŞAĞIYA EKLEDİM

struct Nec{
	int i{7};
	Nec() = delete;  //Eskiden ctor delete/defalt edilirse(user declared) bu durumda aggregate olma özelliği bozulmuyordu
};			 // User provided Ctorda bozmuyordu hatta

Aggreagte olduğunda aggregate init yapılabiliyor.Olmazsa eğer sentaks hatası

int main()
{
	Nec x; //SENTAKS HATASI
	Nec y{}; // C++17 de GEÇERLİ !!!! BU REZALET :D:D:D LEGAL OLMAMALI
}

--------------------

Agregate olma koşullarını C++20 de değiştirdiler. Normalde yapılan eklemeler geçmişte yazılan kodların çalışmasını sağlayacak
şekilde ekleniyor.Artık C++20 de user declared olmamak zorunda yoksa aggregate olmaz.

struct A{
	A() = delete;
};

struct B{
	int x{1};
	B(int) = delete;
};

struct C
{
	int x;
	C();
};

C::C() = default;

int main()
{
	A a; // HATA
	B b{12}; //HATA. C++17 de geçerliydi
	
	//HİÇBİRİ GEÇERLİ DEĞİL
}

Özetle aggregate olma koşulları daha sık hale getirildi.

-----------------------------------------------------------------------------------------------------


2. NEDEN EKLENDİ ? 

Aggregate typelar parantez tokenı kullanılarak initialize edilemiyordu.
Sadece küme poarantezi ile init edilebiliyordu.C++20 ile parantez atomuylada
init edilebilme özelliği getirildi. 
NEDEN EKLENDİ ?  2 tane temel problem vardı.
1- Önişlemci programla uyumsuzluğu. Macroda {} kullanılınca içerideki virgül ayrı kurallara tabi kılınıyor
2- Perfect Forwarding ile ilgili. Küme parantezi ilk init edince perfect forwardingte 
arka taraftaki kodda problem oluşuyır ama () kullanınca problem yok.

YİNE BİR ÖNCEKİ DERSTE YAZILANLAR AŞAĞIYA EKLENDİ

struct Nec{
	int x,y;
};

int main()
{
	Nec nec1{10,20}; // LEGAL
	Nec nec2(10,20);  // C++20 İLE BUDA LEGAL
}
- Öyle yerer varki {} kullanınca sentaks hatası oluyor mesela perfect forwarding. Bir yapı türünü perfect forwarding
  edince sentaks hatası oluşuyordu. 
- Birde Macrolarda hatalar oluşuyor.
#include<cassert>
int main()
{
	using namespace std;
	assert(Nec{1,5}); // Macroda virgül kullanımı macro açılımını bozacak.Parantez olsaydı sentaks bozulmayacaktı
	assert(Nec(1,5)); // Bu Macro açılımını bozmuyor.
}
NOT : Aşağıdakilerde geçerli
int a[]{1,2,3};
int a[](1,2,3);


-----------------------------------------------------------------------------------------------------

3.

struct Nec{
	int a,b,c;	
};

int main()
{
	Nec n1{1,2,3}; // GEÇERLİ
	Nec n2(1,2,3); // C++17 DE GEÇERSİZ C++20 de GEÇERLİ
	
	-------------------------------------------------------------------------------
	
	Nec n3{1,2.2,3}; // Burada narrowing conversion. warning veya sentaks hatası verir
	Nec n4(1,2.2,3); // Burada hata yok.Diagnostic vermiyor.
	
	-------------------------------------------------------------------------------
	
	Nec n5(.b = 6); // C++20 ile geçerli
	Nec n5{.b = 6}; // GEÇERSİZ
}


-----------------------------------------------------------------------------------------------------

4. Life extension {} kullanınca uygulanıyor ama () uygulanmıyor

struct Nec{
	const int &r;	
};

int foo()
{
	return 1;
}

int main()
{
	Nec n{ foo() }; // Kod geçerli ve life extension oldu
	
	Nec n( foo() ); // Legal ama aradaki fark life extensionun yapılmıyor olması.
					// Burada life extension yok.Most Vexing parsı düşünme şimdilik.
}

-----------------------------------------------------------------------------------------------------


5. BRACE ELISION

struct Data{
	int x,y;
	struct Nested{
		int a,b;		
	}z;
};

int main()
{
	struct Data mydata = {1,2,{3,4}}; // GEÇERLİ
	struct Data mydata = {1,2,3,4}; // Brace konulmazsada sentaks hatası değil.
	
	struct Data mydata = (1,2,3,4); // SENTAKS HATASI.Burada Geçerli değil.
									// Bazı drleyiciler burada sentaks hatası olmasın diye
}									// extension olarak bunu verebilirler.


-----------------------------------------------------------------------------------------------------

6. 

struct Data{
	int a;
	int b = 20;
	struct Nested{
		int x;		
	}c;
};

int main()
{
	Data myder{3}; // İlk değer vermediklerimiz 0 oldu. Default member initler değerini aldı.
					// () kullanıncada değişiklik yok.
					
	std::cout << mydata.a << mydata.b<< mydata.c.x;
}

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

7. 

int main()
{
	int a1[]{1,2,3};
	int a1[](1,2,3); // İkiside legal
	
}


struct Nec{
	int a,b;
	struct Nested{
		int x;
	}c;
};

int main()
{
	using namespace std;
	
	auto p = make_unique<Nec>(1,2,Nec::Nested{3}); // make unique perfect forwarding yapıyor
													// C++20 de hata yok. C++17 de hata var
													// Parantezle ilk değer verme sentaksı gelince hata da kalktı C++20 de

	vector<Nec> nvec;
	nvec.emplace_back(1,2,Nec::Nested{3}); //C++20 de geçerli. C++17 Sentaks hatası

}

-----------------------------------------------------------------------------------------------------

8.STRUCTURED BINDING EKLEMESI

class Nec{
	int mx,my;
	friend void func();
	
};

Nec foo();

void func()
{
	auto [x,y] = foo(); // C++17 ile Sentaks hatası. C++20 Geçerli.
						// Yani friend bildirimi C++20 devreye girecek.
	
}

-----------------------------------------------------------------------------------------------------

9. RANGE BASE FOR LOOP EKLEMESI

class Nec
{
public:
	std::vector<int>& get_values(); // implementasyonu yazmayacağız
	
};

Nec get_nec();

int main()
{
	for(auto x : get_nec().get_values()) // DANLING REFERENCE UB.Geçici nesne hayatı uzamıyor.
	{
	}
}

Bunu aşağıdaki gibi yazabiliriz.

int main()
{
	for(auto necx = get_nec(); auto item : necx.get_values())
	{
		//..
		
	}
	
}

-----------------------------------------------------------------------------------------------------

10.

int main()
{
	using namespace std;
	
	std::vector<int> ivec{1,3,5,7,9};
	
	for(std::size_t i{}; auto val : ivec) // indexi döngünün gövdesinde kullanabiliriz.
	{
		std::cout << "{" << i++ << ", " << val << "}\n"; 		
	}
	
}

YUKARIDAKİ AŞAĞIDAKİ GİBİDE YAZILABİLİR AUTO KULLANARAK
for(auto i{0}; auto val : ivec) // indexi döngünün gövdesinde kullanabiliriz.
{
	std::cout << "{" << i++ << ", " << val << "}\n"; 		
}

Scope leakage yok.

-----------------------------------------------------------------------------------------------------

11.

struct Nec{
	int a : 4{2};
	int b : 4 = 5;  // C++20 öncesinde bitfield memberlar için default member init kullanılamıyordu.
					// Ancak bir ctor yazılarak yapılabilirdi.
					// C++20 de GEÇERLİ
};

int main()
{
	
}

-----------------------------------------------------------------------------------------------------

12. TYPENAME KEYWORD KULLANIMI

C++17 de belirli yerlerde template kodlarda dependent template parametresine bağlı türleri 
kullanırken typename keyword kullanımı zorunlu. Bunu kullanmazsak çözünürlük operatörünün
sağındaki ismi bir dependent typename ismi olarak değil, sınıfın static veri elemanı olarak alıyordu
Bu kuralın istisnası yoktu. Öyle yerler varki context ten zaten onun tür bilgisi olduğu açık şekilde 
belli oluyor. Başka ihtimal yok zaten. 

Bu typename kullanılmazsa static veri elemanı olan bir isim olarak alacak derleyici. 
Typename kullanınca onun bir tür olduğunu belirtiyoruz ve burada class keyword kullanılamıyor.

Şimdi belirli yerlerde bu gevşetildi. Zaten başka bir ihtimal yoksa, oradaki ismin
static veri elemanı ismi olma ihtimali yoksa, sadece tür ismi olma ihtimali varsa
typename kullanılmak zorunda değil.

ÖR:

template<typename T>
T::Neco func(); // C++ 17 de hata bu. C++20 de geçerli.Typename e gerek kalmadı
				// Burada T::Neco nun static veri elemanı olma ihtimail yok.


template<typename T>
void func(T::Neco); // Burada C++20 de bile geçerli deil. Burada typename Yazıyoruz
void func(typename T::Neco); // Geçerli


template <typename T>
struct PointerTrait{
	using Ptr = void*;
};

template<typename T>
struct Nec{
	using Ptr = PointerTrait<T>::Ptr; //C++17 Burada da typename zorunlu.
									  // C++20 de Geçerli.typename e gerek yok.
	
	T::Neco foo(T::Ali x); // Burada class scopeta isim kullanıldığı için typename kullanmıyor olmama rağmen
	{						// herhangibir problem yok.
	
	}
};

-----------------------------------------------------------------------------------------------------

13. STATIC CAST

template <typename T>
struct PointerTrait{
	using Ptr = void*;
};

template<typename T>
struct Nec{
	using Ptr = PointerTrait<T>::Ptr; 
	
	T::Neco foo(T::Ali x); 
	{						
		return static_cast<T::Neco>(x); //GEÇERLİ
	}
};

Typename gerekmemesi burada class scope içinde kullanılması durumunda oluyor.

-----------------------------------------------------------------------------------------------------

14. TRAILING RETURN TYPE

template <typename T>
struct PointerTrait{
	using Ptr = void*;
};

template<typename T>
struct Nec{
	using Ptr = PointerTrait<T>::Ptr; 
	
	T::Neco foo(T::Ali x); 
	{						
		return static_cast<T::Neco>(x); //GEÇERLİ
	}
	
	auto f()->Nec<T>::ptr; // Typename e gerek yok burada da.
};

Yani herhangibir başka ihtimal yoksa onun sadece tür olma ihtimali varsa
typename keyworde gerek yok diyor.


-----------------------------------------------------------------------------------------------------

15. NAMESPACE 

namespace A::B::inline C{
	void func();
	
}
	
C namespace i b içerisindeki inline bir namespace 

int main()
{
	A::B::func();
}

-----------------------------------------------------------------------------------------------------

16. ARRAY NEW DEDUCTION

int main()
{
	int a[]{1,2,3}; // Derleyici boyutu anlıyor burada
	
	int *p = new int[]{1,2,3}; // C++17 Sentaks hatası. Size explicit olarak bildirilmeli
				 // C++20 ile deduction yapılma garantsi var.Geçerli
							 
							 
	char *p = new char[]{"necati"}; // C++20 Geçerli
	
	int *p = new int[]{1,2,3}; // C++20 Geçerli
							 
}

-----------------------------------------------------------------------------------------------------

17. NEGATİF SAYILARIN GÖSTERİMİ 2 YE TÜMLEYEN ARİTMETİĞİ İLE YAPILMAK ZORUNDA DEĞİL.

Her ne kadar sistemlerin çok büyük çoğunluğunda negatif sayıların gösterimi 2 ye tümleyen aritmetiği ile yapılsada 
aslında dilin kuralları 2 ye tümleme aritmetiğinin garantş altında olmayışına göre saptanmış.

Bunun sonucunda da C++20 öncesinde bazı durumlar derleyiciye bağlı, bazı durumlarda undefined behaviordu
C++20 de radikal karar aldılar. Negatif sayıların tümleyeni 2 ye tümleyen aritmetiği ile yazılmak zorunda.

Dolayısıyla buna güvenerek yazılan kodlar UB İmplementatiton defined oluşturmayacak.

NELER BUNLAR ?
1 - Negatif tamsayıların sola kaydırılması.

int main()
{
	int x = -1;
	x <<= 1; //Daha önce bu UB idi şimdi böyle birşey sözkonusu değil.
	
	std::cout << "x = " << x << '\n';	
}

11111111111111
11111111111110  bu hale geldi. Daha önce bu UB idi.

2 - 
int main()
{
	int x = INT_MAX;
	
	x <<= 1; // GEÇERLİ	
}

3 - Negatif sayıların sağa kaydırma işleminde, soldan yapılan besleme 0,
Bir durum vardı o durum implementation defined.
Tür unsigned ise besleme 0 ile yapılıyor
işaretli fakat pozitif ise besleme yine 0 ile yapılıyor.
İşaretli ve negatif ise soldan yapılan besleme implementation defined.
Aritmetik mi logic mi ? C++20 öncesinde Derleyiciye bağlı.

int main()
{
	int x = -1;
	if((x >> 1) == 1) // C++20 öncesinde burası Derleyiciye bağlı.2 ye tümleyen aritmetiği geldi. Ub değil
	{
		std::cout << "dogru\n";  // Doğru kısmına girdi.
	}
	else
	{
		std::cout << "yanlis\n";
	}
}

-----------------------------------------------------------------------------------------------------

18. NOTHROW EXCEPTION DURUMU

Derleyicinin yazdığı move ctor'ın noexcept olup olmadığı bu sınıfın içerisindeki elemanlara bağlı. 
Derleyici sınıfın elemanlarına bakıp eğer bunlarında move ctoru noexcept ise derleyicide noexcept
olarak default ediyor.

class Myclass{
public:
	Myclass(Myclass&&) = default;
};


int main()
{
	static_assert(std::is_nothrow_move_constructible_v<Myclass>); // Bu ctor no except.
}


Değiştirelim Örneği

struct Data{
public:
	Data(Data&&) noexcept(false);
};

class Myclass{
public:
	Myclass(Myclass&&) = default;

private:
	Data x;
};

int main()
{
	static_assert(std::is_nothrow_move_constructible_v<Myclass>); // HATA VAR. Kırmızı oldu. Elemanlarda data var ve Data olan elemanın
																	// Move Ctoru noexcept değil.
}


C++20 ILE HEM DEFAULT EDIP HEMDE EXCEPTION KOŞULUNU KOYABILIYORUZ.

struct Data{
public:
	Data(Data&&) noexcept = default; // Noexcept olarak default edebiliyoruz.Noexcept garantisi ile default etti
	// Data(Data&&) noexcept(false) = default; // Noexcept(false) olarakta default edebiliyoruz.Noexcept garantisi olmadan default etti
};


-----------------------------------------------------------------------------------------------------

19. EXPLICIT CTOR

class Myclass{
public:
	Myclass();
	explicit Myclass(int); // Explicit ctor burada int türünden myclass türüne olan dönüşümleri otomatik yapmıyor.
};							// type cast ile yapılabilir.

int main()
{
	Myclass m;

	m = 5; // HATA

	Myclass ny = 23; //copy init Sentaks hatası yine
}

C++20 ile explicit ctoru bir şarta bağlayabiliyoruz.Buna conditionally explicit ctor. 
Burada ya constructor template olmalı yada doğrudan sınıf template olmalı.

template<typename T>
struct Nec{
	explicit(true) Nec(T); // explicit içerisinde boolean ifade olmalı.Burada ctor explicit
	explicit(false) Nec(T); // Burada ctor explicit değil.

	explicit(std::is_integral_v<T>)Nec(T);
};

int main()
{
	Nec mynec = 12; //Sentaks hatası
	Nec mynec = 1.2; //Geçerli :D:D

}


ÖR 2:
template<typename T>
struct Nec{
	//explicit(true) Nec(T); // explicit içerisinde boolean ifade olmalı.Burada ctor explicit
	//explicit(false) Nec(T); // Burada ctor explicit değil.

	//explicit(std::is_convertible_v<T,int>)Nec(T); // T türü int e dönüştürülebiliyor mu sınanıyor
	explicit(!std::is_convertible_v<T,int>)Nec(T); // T türü int e dönüştürülebiliyorsa false olacak çümkü ! var 
};

int main()
{
	Nec mynec = 'A'; // True oldu ve ctor explicit oldu. HATA
}

-----------------------------------------------------------------------------------------------------

20.
C++20 öncesinde adres türlerinden bool türüne dönüşüm narrowing conversion sayılmıyordu
Artık narrowing conversion.

struct Nec
{
	int x;
	bool flag;
};

int main()
{
	void *vptr{};
	Nec nec{12,vptr}; // Void * dan boola dönüşümün narrowing conversion olduğunu warning olarak verdi derleyici
					  // C++20 öncesinde narrowing conversion sayılmıyordu.


	int x = 10;

	bool b{&x}; // Burası doğrudan sentaks hatası

}

-----------------------------------------------------------------------------------------------------

21. 
Type alias kullanınca deductiopn yapılamıyordu C++17 de.

#include<utility>

template <typename T>
using ipair = std::pair<int,T>;

int main()
{
	using namespace std;

	pair<int,double> x;

	ipair<double> ; // bu bir yukarıdaki ile aynı oldu pair<int,double> oldu

	-----------------------------------------------------------

	ipair p{1,2.3}; // Visual studio derleyicisi hata veriyor ama Gcc de hata yok.
					//	Normalde bu deductionu yapmalı. C++20 ile geçerli olmalı.Wandboxta test ettik.
}


=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================
=============================================================================================================================================


CONCEPT 
-------
C++20 ile gelen belkide en önemli özellik.
Generic programlamada generic/meta bir kod oluşturuyoruz.
Birçok durumda oluşturduğumuz template'lerde template parametreleri için bazı kısıtlamalar koymak istiyoruz.
Template parametresi tamsayı olmak zorunda gibi. Çok basit kısıtlamadan çok komplex kısıtlamaya kadar gidebilir.

Eski teknikler var. Bunlar birçok problemi barındırıyor.

Eski Tekniklerin Dezavantajları
-------------------------------
1. Çok zor ve karmaşık.Template kodları yazabilmek için uzman seviyesinde olmak gerekiyor
2. Kısıtlamalar çiğnendiğinde yada gerekli koşullar sağlanmadığında verilen hata mesajları çok karmaşık ve uzun.

requirement -> Gereksinim
constraint -> Kısıtlama

Concept Avantajları
-------------------
1 - Kısıtlamalar ortak bir sentaksla yapılıyor.Artık çok daha kolay
2 - Hata mesajları çok daha kolay ve kısa hale geldi.
3 - Overloading konusunda da çok büyük avantajlar getirdi.

Bunu tam olarak anlamak için C++ standartları tarafından belirlenen constraint'in anlamına bakalım.
CONSTRAINT = Derleme zamanında ele alınan boolean ifadeler.Yani derleme zamanında değeri true/false olacak.	
	     Bu bu constraint olarak kullanılan koda bağlı olarak derleyici sözkonusu template'ten kod üretilsin mi
             üretilmesin mi bunun kararını verecek.

C++20 ile 2 tane yeni keyword eklendi. Biri concept diğeride requires.
1 - concept
2 - requires

Template parametrelerinin sağlaması gereken koşulları belirtiyoruz burada.
Bu kısa bir konu değil.Toplamda 4-5 saati bulacak bir konu gibi duruyor.
Concept ile standart kütüphaneyede eklemeler yapıldı.Template parametrelerinin sağlaması gereken 
koşullar standart kütüphanedeki conceptler ile sağlanıyorsa ideali bunları kullanmak.

Concept in kendisi doğrudan bir constraint oluşrutuyor zaten.yani varolan bir concepti bir template üzerinde
belirli bir sentaksa uyarak oluşturabiliyoruz.
Header file concepts

-----------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------

#include<iostream>
#include<concepts>

1. KULLANIM
-----------
template<typename T> yazardık eskiden, artık aşağıdakini kullanıyoruz.

template <std::integral T>	// alternatif sentakslardan biri bu.Burada std::integral bir concept !!!!!!!!!
void func(T x)
{

}

Burada T tamsayı türlerinden biri değilse yani bu conceptin requirementlarını sağlamıyorsa doğrudan bu template devre dışı kalacak.
Devre dışı kalması eğer başka bir alternatif yoksa func call geçersiz kılınacak ve sfinae de olduğu gibi hata alınacak.
Başka overloadlar varsa onlara bakılacak.
Buarada kendimizde requirementları belirtebiliriz.Çok daha karmaşık yapabiliriz.
Burada func(2.3); ile çağrılınca T double olur ve function overload setten çıkacak
sfinae out gibi. Bunlar detaylı görülecek.
T burada integral conceptinin koşullarına uyan bir tür olmak zorunda.

Tekrar : std::integral concepts header file içinde oluşturulan conceptlerden biri.
Standart conceptlerin büyük çoğunluğu concepts içinde tanımlanmış.Hepsi değil ama.


2. KULLANIM
-----------
void func(std::integral auto x)		// Template <typename> yok !!! C++20 ile gelmişti buradaki auto
{
	
}


3. KULLANIM
-----------
template<typename T>
void func(std::integral<T> x)	// Bu kısımda problem olabilir.İncelenecek
{

}

-----------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------

CONSTAINT OLUŞTURMA YOLLARINDAN BIRIDE REQUIRES KEYWORDÜ ILE OLUŞTURULAN YAPI.
Requires keywordü ile oluşturulan ifadeyse bir concept oluşturuyoruz. Constraint oluşturmanın yollarından biri concept diğeride requires keyword ile 
oluşturulan bir requires clause.Argüman olarak 3 farklı sentaks olabiliyor.


SENTAKS - 1
-----------
template<typename T>
requires (sizeof(T) > 2)  // doğruysa constraint sağlanacak yanlışsa sağlanmayacak.ad-hoc boolean expression diyorlar
void func(T x)
{

}

SENTAKS - 2
------------
template<typename T>
void func(T x) requires (sizeof(T) > 2) // requires ifadesi bir compile time expression olmalı
{

}

Her iki durumda da requires ı izleyen ifade compile time da true olmak zorunda.
İster 1. ister 2. sentaks kullanalım anlamsal farklılık yok.
Bu template bu constrainti sağlamak zorunda.

ÖR:
template<typename T>
void func(T x) requires (std::is_integral_v<T>)
{

}

ÖR:

template<typename>
requires (std::is_integral_v<T>)
class Myclass{
	
};

T türü tamsayı türlerinden biri olmak zorunda

int main()
{
	
	Myclass<int> x;
	Myclass<double> x; // Constraint sağlanmadığı için sentaks hatası olacak.
}

-----------------------------------------------------------------------------------------------------------------------------------------------

Requires clause oluşturmanın birden fazla yolu var. Bunlardan biri ad-hoc 
constant expression oluşturmak.Requires keywordü izlen bir conceptin kendiside olabilir.


tempalte <typename T>
requires (std::is_integral_v<T>)
void foo()
{

}

-----------------------------------------------------------------------------------------------------------------------------------------------

REQUIRES CLAUSE
--------------

template<typename T>
requires(sizeof(T) > 8) && 
	requires {
		typename T::value_type;  // böyle bir nesnted type olmazsa constraint sağlanmayacak.
		*p; 	 // bu ifade de legal olmak zorunda. ; ler ile ayırarak birden fazla require expression ekleyebiliriz.
		p[0];    // bununda geçerli olması gerekiyor
	} 
	&& std::integral<T> // en yukarıda requires olduğundan burada requires'a gerek yok.


REQUIRES IÇIN ALIAS OLUŞTURMAMıZA GEREK YOK. ZATEN REQUIRES IÇIN OLUŞTURULAN ALIAS
CONCEPTIN KENDISI. CONCEPTIN AMACI CONSTRAINTLERI ISIMLENDIRMEK. !!!!!!!!!!

-----------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
concept myconcept = sizeof(T) > 10;

Concept zaten constraintlerin isimlendirilmiş biçimi
Conceptleri oluştururken requires expressionları yada başka conceptler 
kullanılacak

template<typename T>
concept myconcept = sizeof(T) > 10 && std::is_integral_v<T>;

Concept requirementların toplamının isimlendirilmiş hali

-----------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
requires(sizeof(T) > 10) && requires(std::is_integral_v<T>);  

//Konsept kullanınca isimlendirmiş oluyoruz.

-----------------------------------------------------------------------------------------------------------------------------------------------

Requires Clause'a devam

template<typename T>
requires std::integral<T> || std::floating_point<T>  //bu iki satır sadece requires clause
void func()
{

}

//Data önce bu requires clause'u kullandığımızy erleri yazdık yukarıda. return değeri gibi...

int main()
{
	func<int>(); // Geçerli
	func<double>(); // Geçerli
	func<int *>(); //Sentaks hatası
}

-----------------------------------------------------------------------------------------------------------------------------------------------

NELER KULLANILABİLİR REQUIES CLAUSE'TA?
--------------------------------------
template <typename T>
requires (sizeof(int) > 4) && sizeof(T) > 16)

-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
requires (std::is_pointer_v<T>) // Bu constraint template paranmetresinin pointer türlerinden olması gerektiğini söylüyor

-----------------------------------------------------------------------------------------------------------------------------------------------

Birden fazla template parametreside kullanılabilir.

template <typename T, typename U>
requires (sizeof(T) > sizeof(U))

-----------------------------------------------------------------------------------------------------------------------------------------------

Conceptlerin kendisini kullandığımız zamanda birden fazla tür parametresi olabilir

template <typename T, typename U>
requires std::convertible_to<T,U> // bu constraintin sağlanması için T türünden U türüne dönüşüm olmalı.

-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T, typename U>
requires (std::is_convertible_v<T,U>)  // Burada kullandığımız type trait.concept değil.

-----------------------------------------------------------------------------------------------------------------------------------------------

Requires template Tür parametresine bağlı olmak değil 

template <typename T, typename U>
requires (sizeof(int) < sizeof(long))  // geçerli

-----------------------------------------------------------------------------------------------------------------------------------------------

Nontype Parametre içinde Constraint oluşturulabilir.

template <std::size_t n>
requires (n > 20)
class Myclass{

};

int main()
{
	Myclass<200> m; //GEÇERLI
	Myclass<20> m; // SENTAKS HATASI
}

-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
requires(std::is_pointer_v<T> || std::is_same_v<T,std::nullptr_t>)
class Nec
{

};

int main()
{
	Nec<int> x; // Sentaks hatası. Pointer veya nullptr_t türü değil.
	Nec<nullptr_t> y; // Geçerli.
	Nec<int *> y; // Geçerli.
}

-----------------------------------------------------------------------------------------------------------------------------------------------

String türüne dönüştürülemeyecek tür 

template <typename T>
requires(!std::is_convertible_v<T, std::string>)
class Nec
{

};


Concept kullanabiliriz. Concept olunca ismi değişiyor.
std::convertible_to


template <typename T>
requires(!std::convertible_to<T, std::string>) // ! operatörünü kullanınca concept olsada parantez içinde olmak zorunda.
class Nec{
	
};

-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
constexpr T foo(T x, T y);

template <typename T, int x, int y>
requires (foo(x,y) > 10 )	// constexpr funcı sabitlerle çağırınca compile time ifadesi olmuş oluyor.
class Myclass { 

};

-----------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------

REQUIRES EXPRESSION
-------------------
Requires closse ta bir sabit ifade ve bir concept kullanabiliriz,
3. ihtimal özel bir sentaksla bir veya daha fazla koşulu velirtecek yapıyı oluşturmak.
buna requires expression deniyor.

SENTAKS
-------
template <typename T>
	requires {
	expr1;			//Burada ; ile ayrılmış ifadeler olacak 
	expr2;			// ; ler ile ayrılmış her yapı bir constrainti belirleyecek
	// ..
	}

ÖR:

template <typename T>
concept Neco = requires(T x){
	
	typename T::value_type; // T türü hangi türse onun value type türü olmak zorunda.
				// value type isimli nested type yoksa doğrudan concept koşulları sağlanmamış olacak

	x + x; // Sentaks hatası olmamalı.Herhangibir ifade yazdık mesela burada.

	x > x; // Bu ifade doğru değil ama sentaks hatası yok. Önemli olan sentaks hatası olmaması.

	x == nullptr; // x nullptr olmak zorunda değil, karşılaştırma yapılabilmeli demek
	
	x.foo(); // x in foo func ı olmak zorunda.

} && diğer requires clauselar birleştirilebilir;


static_assert(Neco<int>); //Sentaks hatası çünkü int türü neco conceptinin koşullarını sağlamadı.

-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
concept neco = requires(T x){
	x.foo();
};

struct Data{
	 void foo();
};

static_assert(Neco<Data>); // Data nın foo func ı var. Bir hata olmayacak.

*/
