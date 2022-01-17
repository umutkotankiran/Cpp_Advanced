/*

Conceptlerde kalmıştık. En önemli eklemelerden biri. 
Standart library yi tamamen değiştirdi.Bununla birlikte ranges library geldi.
Hatta three way comparisonu eklersek, STL eski STL değil artık.

Artık algorithm i kullanacağız ama daha çok ranges kullanacağız belkide.
Derleyici implementasyonuda giderek tamamlanıyor.

Templatelerde parametreler üzerine kısıtlama getirebiliyorduk. Derleyicinin kod üretebilmesi için
instantiate edebilmesi için öncelikle dilin sentaksına uygun şekilde konulmuş kısıtlamalar varsa
o kısıtlamalara uyulmak zorunda.Aslında constraint burada template parametrelerinin uyması gereken
koşullar anlamında kullanılıyor.

Daha önce kullanılan çok karmaşık tekniklerin kullanımını gereksiz kılıyor.
mesela sfinae.Artık bu tip teknikler yerine constaint ler oluşturuyoruz.
Kodlar artık daha şık ve okunabilir. Birçok yan faydası var.Kodlar daha açık ve okunabilir.
Artık constaintler doğrudan template interfaceinin bir parçası.Templateler overload edilebiliyor.
Sınıf şablonlarında partial specializetion oluşturabiliyoruz. Hata mesajlarının daha iyi hale geliyor.

Concept constaintlerin isimlendirilmiş halidir.

Conceptin kendiside bir templatedir.

Çoğu concept type traitslerdeki traitlerden hareketle oluşturulmuş.
Bir kısmıda tamamen trait in kendisini kullanıyor.

----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

REQUIRES EXPRESSION
-------------------

Sentaks
-------
requires { requirement-seq}
requires(parameter-list-optional) {requirement-seq}


- En çok kullanılan türe ilişkin bir bildirimin bulunması

template<typename C>
concept nec = requires{
	typename C::value_type;
};

struct A
{
	using value_type = int;
};

template <typename T>
requires nec<T> 	 // requires clause diyorki nec<T> true olacak.T türünün nec conceptine satisfied etmesi demek bu
class Myclass{
	//
};

int main()
{

	// Bu sınıf şablonundan sınıf oluşturduğunda eğer argüman olarak verdiğim türün value typeı yoksa
	// concept satisfied olmayacak.

	Myclass<int> x; // SENTAKS HATASI.int in value_type olmadığından satisfied olmadı.

	----------------------------------------------------------------------------------

	Myclass<A> x; // GEÇERLİ

}

-------------------------------------------------------------------------------------------------

ÖR:

template<typename C>
concept nec = requires{
	
	typename C::value_type::pointer;  // Burada value typeın pointer türüde olmalı
	typename C::value_type::reference; // Mesela yukarıdaki satisfied ama bu değilse yine SENTAKS HATASI

};

-------------------------------------------------------------------------------------------------
ÖR:

template<typename C>
concept cnec = requires{
	typename C::key_type;
	typename C::value_type;

};

template<typename T>
requires cnec<T>
void func(T); // func bildirimi yaptık

#include<map>

int main()
{
	using namespace std;
	std::map<int,double>mx;
	string s;

	func(mx); //GEÇERLİ
	func(s); //SENTAKS HATASI çünkü stringin key ve value type ı yok
}

-------------------------------------------------------------------------------------------------

DİKKAT !!!!!!!!

template<typename T>
class Tclass{

};

struct A{};

template<typename T>
concept nec = requires {
	typename Tclass<T>;  // BU DIYORKI BÖYLE BIR TÜR OLABILMELI 
};

int main()
{
	static_assert(nec<int>); // GEÇERLİ

	static_assert(nec<A>); // GEÇERLİ
}

-------------------------------------------------------------------------------------------------

//Burada template argümanı olarak kullanılacak tür, integral conceptini satisfied etmeli
template<std::integral T>
class Tclass{
	
};

struct A{};

template<typename T>
concept nec = requires {  // Nec concepti diyorki Tclass<T> legal olmalı diyor.T classta yukarıdaki gibi legal olarak açılımı yapılmalı
	typename Tclass<T>;  
};

int main()
{
	static_assert(nec<int>); //GEÇERLI

	static_assert(nec<A>);  // SENTAKS HATASI
}

-------------------------------------------------------------------------------------------------

STL DE RANGES TE KULLANMAYA BAŞLAMIŞLAR

#include<concept>
#include<ranges>

template<typename T1, typename T2>
concept nec = requires{
	typename t1::value_type;
	typename std::ranges::iterator_t<T1>;
	typename std::ranges::iterator_t<std::vector<T1>>;  
	typename std::common_type_t<T1, T2>;	 // Hepsinin sağlanması lazım
};

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

BIR ÇOK DURUMDA CONSTAINT I EN IYI IFADE ETMENIN YOLU, IFADENIN GEÇERLI OLMASI !!!!!!!!!!!!!!!!

x + y; x int y double ise geçerli
x + y; x int y string ise geçersiz. sting ile int i toplayacak func yok.

template parametrelerine ilişkin belirli ifadelerin geçerli olmasına yönelik constraint koymak
işi çok kolaylaştırıyor.Concept öncesi çok zordu

template <typename T>
concept nec = requires(T a, T b)
{
	a + b;  // Derleyici bunun yapılabilir olup olmadığına bakıyor.Bir işlem kodu üretmiyor
};

static_assert(nec<int>); // Legal
static_assert(nec<int*>); // SENTAKS HATASI int * + int * toplanamaz.

-------------------------------------------------------------------------------------------------

template <typename T>
concept nec = requires(T a, T b)
{
	a - b;
};

static_assert(nec<int*>); // Geçerli 2 ptr birbirinden çıkartılabilir.

-------------------------------------------------------------------------------------------------

AŞAĞIDAKİ NE DEMEK ? 

template <typename T>
concept nec = requires(T p)
{
	p == nullptr; // UNEVALUATED EXPRESSION.IFADENIN KENDİSİ GEÇERLİ OLMALI.TRUE FALSE OLMASI ÖNEMSİZ
};

static_assert(nec<int*>);

static_assert(nec<std::optional<int>>);  //optional türden nesne nullptr ile kıyaslanamaz

static_assert(nec<std::optional<int*>>);  // Yukarıdaki de *p == nullptr olsaydı geçerliydi.

static_assert(nec<std::unique_ptr<int>>);  //GEÇERLİ

-------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
concept nec = requires(T1 x, T2 y)
{
	x > y && *x < *y;
};

static_assert(nec<int,double>); // SENTAKS HATASI
static_assert(nec<int*,int*>); // GEÇERLİ

int main()
{

}

-------------------------------------------------------------------------------------------------

template <typename T>
concept nec = requires(T x)
{
	std::cout << x;  // yani x insertable olacak
	std::cout >> y; // bununda legal olması gerekiyor.
};

struct A{};
static_assert(nec<A>); //Sentaks hatası

-------------------------------------------------------------------------------------------------

REQUIRES EXPRESSION İÇERİSİ BOŞ BIRAKILAMIYOR

template <typename T>
concept nec = requires(typename T::value_type x)
{
	// Boş olduğu için SENTAKS HATASI en azından içerisine true; yazılabilir
}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

REQUIRES CLAUSE İSE AŞAĞIDAKİ. REQUIRES EXP İLE KARIŞTIRILMASIN

template<typename T> requires Addable<T> // requires-clause, not requires-expression
T add(T a, T b) { return a + b; }

template<typename T>
concept Addable = requires (T x) { x + x; }; // requires-expression

-------------------------------------------------------------------------------------------------

template <typename T>
requires std::integral<T> &&  ...

Yada

template<typename T>
requires (sizeof(T) > 4) &&			// Bu bir requires clause 
		  std::integral<T> &&		// bu bir concept
		  requires{			// requires expression
				typename T::value_type;
		  }

void func(T x); // Burada T tüm constaintleri sağlamak zorunda

-------------------------------------------------------------------------------------------------

ÖR

template <typename T>
requires requires(T x)		// 2 requries kullanınca hata yok burada. 1. requires, requires clause un requiresı.
{ x + x; }
void foo(T x)
{
	
}

-------------------------------------------------------------------------------------------------

template<typename T, typename U>
concept myc = requires(U x, U y)
{
	*x;
	y[0];
	x->geta();
	*x > y;
	x == nullptr;
	x = nullptr;
};

Geçerli olması için hiç hata vermemeli.

-------------------------------------------------------------------------------------------------

DİKKAT!!!!

template<typename T>
concept nec = requires{
	std::integral<T>;  // dikkat başında typename yok
};

Burada T integral bir tür mü kontrolü yapılmıyor.T ne olrsa olsun bu ifade yazılabilir.

int main()
{
	static_assert(nec<double>); //Fail değil.Çünkü burada kontrol edilen ifadenin doğruluğu değil
				    // sentaks hatası veriyor mu vermiyor mu konusu
}

MOLA

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

NESTED REQUIREMENTS
-------------------
template<typename T>
concept myc = requires{
	requires std::integral<T>; // T nin integral conceptini satisfied etmesi gerekiyor.
};

NOT !!!!!!!!!!!!!!!!!!!
Yukarıdaki  requires std::integral<T>; // BURASI REQURIES CLAUSE
			
			requires{	// BURASI REQUIRES EXPRESSION
				requires std::integral<T>; 
			};

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

COMPOUND REQUIREMENTS
---------------------

{expr}noexcept(optional) -> concept
{expr}noexcept(optional)  bu da geçerli. Örneklerde pek yapmadık.


Küme parantezi içerisine bir ifade yazıyoruz ve -> yazıp sağ tarafa yazdığımız concepti( concept olmak zorunda sağ taraf)satisfied etmek zorunda

{x == x} -> std::convertible_to<bool>; // x == x den elde edilen ifade türü bool türüne dönüştürülebilecek tür olmalı demek.
					// Yani x==x ten gelen ifadenin türü template'te 1. argüman olarak yazılıyor.
					// std::convertible_to<decltype(x == x),bool> Gibi birşey oldu

{x == x} -> bool; //SENTAKS HATASI CONCEPT OLMAK ZORUNDA SAĞ TARAF

--------------------------------------------------------------------------------------------------

ÖR:

template <typename T>
concept nec = requires(T x) {
	{ &x } -> std::input_iterator;  // Bu ifade bu constrainti sağlamak zorunda
};

--------------------------------------------------------------------------------------------------

ÖR:

template<typename T>
concept nec = requires(T c){
	{*c.begin()} -> std::convertible_to<typename T::value_type>;  // c yi dereference edip begin funcını çağırınca 
								      // buradan elde edilen tür sağ taraftaki concepti satisfied
								      // etmek zorunda.Yani T nin value_type ına dönüştürülebilir olmalı.
								      // Burada da *c.beginin türü convertible_to ya ilk argüman olarak veriliyor
								      // 2. argüman zaten var.
								      // std::convertible_to<decltype(x == x),bool>

	{c + 5} -> std::integral; // c + 5 ifadesi integral conceptini satisfied etmek zorunda.Yani c+5 in türü 
				  // std::integral conceptinin tür argümanı oluyor.
				  //std::integral<decltype(c + 5)> gibi bir ifadeye dönüşüyor aslında

};

--------------------------------------------------------------------------------------------------

template <typename T,typename U>
concept Nec = requires{
	
	typename T::xtype;
	typename U::ytype;

};

template <typename T, typename U>
concept nec = requires(T x, U y){
	{x + y } -> Nec<double>;  // x + x ifadesinin türü Nec conceptini satisfied etmeli. 
				  // Nec<int,double> gibi bir ifade gönderirsek bunların int::xtype double::type valid olmalı gibi...
}; 

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

KULLANIM SENARYOLARI

1 - DOĞRUDAN REQUIRES CLAUSE

template <typename T>
requires std::floating_point
void func(T x);

2 - TRAILING REQUIRES CLAUSE

template <typename T>
void func(T x) requires std::integral<T>  // Func parametre parantezinden sonra kullanıldı aynı şey.
{
	
}

3 - CONSTRAINED TEMPLATE PARAMETER
	
template te class veya typename kullanmak yerine doğrudan concept kullanıuyoruz. Func ve class template te olabilir.

template <std::integral T>
class Myclass
{
	
};

int main()
{
	Myclass<double> x; //SENTAKS HATASI concepti sağlamıyor.
}

---------------------------------------------------------------------------------------------------

template<std::convertible_to<int> T>
class Myclass
{
	
};

int main()
{
	Myclass<double> x; //GEÇERLİ
}

---------------------------------------------------------------------------------------------------

template<std::convertible_to<int> T>
class Myclass
{
	
};

int main()
{
	Myclass<std::string> x; //SENTAKS HATASI. string, int türüne convertible değil.
}

---------------------------------------------------------------------------------------------------

template<std::convertible_to<int> T, std::integral U>
class Myclass
{
	
};

---------------------------------------------------------------------------------------------------

template <auto x>
class Myclass{
	
};

int main()
{
	Myclass <10>; // auto ya int geldi.bu template nontype parametreye sahip oldu.
					// C++20 ile artık nontype argüman sadece tamsayı değil reel sayı ve literal typelarda kullanılabilir.

	Myclass<1.2>; // buda geçerli.
}

---------------------------------------------------------------------------------------------------

template<typename T, std::convertible_to<T> auto x> // dikkat nontype olarak kullanılacak türün convertible_to<T> türünü satisfied etmesi gerekiyor
class Myclass{					
	
};

int main()
{
	Myclass<int, 5> x; // Geçerli. VİSUAL STUDIO BURADA HATA VERİYOR AMA HATA YOK !!!!!!!!!!!!!!!!!!!!!
	Myclass<std::string, 5> y; // string türü int e dönüşemediği için sentaks hatası
}

---------------------------------------------------------------------------------------------------

Func return değeri auto ile yazılabilir.

template<std::integral T, std::integral U> // ikiside integral olmalı

---------------------------------------------------------------------------------------------------

ABBREVIATED TEMPLATE SYNTAX

void func(auto x) // auto var ama bu bir template
{
	
}

template<typename T>
void func(T x) bununla yukarıdaki aynı

---------------------------------------------------------------------------------------------------

void func(auto x, auto y)
{
}

int main()
{
	func(3, 3.4); // GEÇERLİ.herbir auto farklı bir template tür parametresine karşılık geldiğinden geçerli
}

---------------------------------------------------------------------------------------------------

auto func(std::integral auto x, std::floating_point auto y)
{

}	// Aşağıdakinin kısa yazımı

template <std::integral T, std::floating_point U>
auto func(T x, U y)
{
	
}

---------------------------------------------------------------------------------------------------

Aşağıda da func return değeri türününde integral conceptini sağlaması gerekiyor.

std::integral auto foo(std::integral auto x)
{
	return x;
	/ return x + 1.2;  deseydik sentaks hatası olurdu
}

int maibn()
{
	foo(34); // geçerli
}

---------------------------------------------------------------------------------------------------

AŞAĞIDAKI COMPILE TIME A YÖNELIK BIR SEÇIM

template<typename T>
void func(T x)
{
	if constexpr(std::integral<T>)
	{
		std::cout << "integral type\n";
	}
	else if constexpr(std::floating_point<T>)
	{
		std::cout << "floating type\n";
	}
	else
	{
		std::cout << "other type\n";
	}
}

int main()
{
	func(12);
	func(1.42);
	func(nullptr);
}

---------------------------------------------------------------------------------------------------

AUTO KULLANILAN HERYERDE BIR CONCEPTI KULLANABILIRIM

int main()
{
	std::integral auto x = 10; // Geçerli
	std::integral auto x = 1.0; //Sentaks hatası
}

---------------------------------------------------------------------------------------------------

RANGE BASE FOR LOOPTA KULLANILABİLİR

int main()
{
	std::vector<int> ivec(10);

	for(std::integral auto x : ivec) // GEÇERLİ
	{
		
	}

	-------------------------------------------
	
	std::vector<float> ivec(10);

	for(std::integral auto x : ivec)  //SENTAKS HATASI
	{
		
	}
}

---------------------------------------------------------------------------------------------------

class Myclass{
public:
	// void func(auto x) // bu üye func ama member template auto dan ötürü.
	// { }

	void func(std::integral auto x);

};

int main()
{
	Myclass m;

	m.func(10); // GEÇERLI

	m.func(1.20); // SENTAKS HATASI
}

---------------------------------------------------------------------------------------------------

template <std::integral ...Args>
bool All(Args ...args)
{
	return (... && args); // unary left fold bu
}

int main()
{
	std::cout << std::boolalpha;
	
	std::cout << All(12,3L,'A',true); // GEÇERLİ

	std::cout << All(1.2,3L,'A',true); // Constrainte takıldı.SENTAKS HATASI
}

---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------

CONCEPTLER OVERLOADINGTE KULLANILABİLİR.REQUIRES CLAUSE TA KULLANILABİLİR TABİ

int main()
{
	using namespace std;

	vector<int> ivec(100);

	auto iter = ivec.begin();
	
	advance(iter,5); // kod seçimi burada compile time da yapılıyor. Bunun içinde concept kullanalım
}


template<std::random_access_iterator Iter>
void Advance(Iter&, int n)
{
	std::cout << "Random Access iterator\n";
}

template<std::bidirectional_iterator Iter>
void Advance(Iter&, int n)
{
	std::cout << "Bidirectional iterator\n";
}

template<std::forward_iterator Iter>
void Advance(Iter&, int n)
{
	std::cout << "Forward iterator\n";
}

BU FUNCLARA YAPILAN ÇAĞRIDA CONSTRAINTLER SAĞLANMAZSA FUNCLAR YOK SAYILIYOR. SFINAE DEKI GIBI BIR DURUM VAR !!!!!!!!!!!!!!!!!!!!!!!!!!!

int main()
{
	vector<int> c(100);

	auto iter = c.begin();

	Advance(iter,5); // random access overload seçilir.Random access değilde bidirectional olsaydı o seçilirdi yada forward iterator seçilirdi

	-------------------------

	list<int> c(100);

	auto iter = c.begin();

	Advance(iter,5); // bidirectional overload seçilir

}

---------------------------------------------------------------------------------------------------

void func(std::integral auto x)
{
	std::cout << "1\n";
}

void func(auto x)
{
	std::cout << "2\n";
}

void func(double x)
{
	std::cout << "3\n";
}

int main()
{
	func(1); // 1 çağrılır

	func(1.3F); // 2 çağrılır
}

MOLA

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

template <typename T>
concept nec = sizeof(2) > 2;

void func(nec auto x)
{
	std::cout << "1";
}

template <nec T>
requires std::integral<T>
void func(T)
{
	std::cout << "2";
}

int main()
{
	func(12);  // hem 1 hemde 2 geçerli.İkiside varsa 2 çağrılır.Partial ordering rulesa benziyor kurallar
				// spesifik olan seçilir.
}

-------------------------------------------------------------------------------------------------------------------

template <typename T>
struct Myclass
{
	Myclass()
	{
		std::cout << "primary template\n";
	}
};

template <std::integral T>  // BURASI PARTIAL SPECIALIZATION
struct Myclass<T>
{
	Myclass()
	{
		std::cout << "partial specialization\n";
	}
};

int main()
{
	Myclass<double> m1; // primary template
	Myclass<int> m2;  // partial specialization
}

-------------------------------------------------------------------------------------------------------------------

template<typename T>
concept Pointer = requires(T p)
{
	*p;
	p[0];
	p == nullptr;
	{p < p}noexcept -> std::convertible_to<bool>;
};

auto min_value(auto a, auto b) // Burada aynı türden olması gerekmiyordu a ve b nin
{
	std::cout << "1\n";
	return a < b ? a : b;
}

auto min_value(Pointer auto a, Pointer auto b)		// her iki parametreninde türünün pointer conceptini satisfied etmesi gerekli
requires std::three_way_comparable_with<decltype(*a),decltype(*b)>	 	// aynı zamanda requires clause ile bir kısıtlama daha getirebilriz.
{
	std::cout << "2\n";
	return min_value(*a, *b);	
}

int main()
{
	int a = 30;
	int b = 45;
	double d = 2.3;

	std::cout << min_value(a, b) << "\n"; // 1. çağrılır

	-----------------------------------------------------------------------------------

	int *p1 = &a;
	int *p2 = &b;

	std::cout << min_value(p1, p2) << "\n"; // 2. çağrılır

	-----------------------------------------------------------------------------------

	int **ptr1{&p1};
	int **ptr2{&p2};

	std::cout << min_value(ptr1, ptr2);  // 1. çağrılır. ?????????????????????????? SOR

}

-------------------------------------------------------------------------------------------------------------------

void print(std::signed_integral auto i)
{
	std::cout << "signed integral : " << i << '\n';
}

void print(std::unsigned_integral auto u)
{
	std::cout << "Unsigned integral : " << u << '\n';
}

void print(auto x)
{
	std::cout << "Non-integral : " << x << '\n';
}

int main()
{
	print(1u); // unsigned integral
	
	print(1.2); // non integral. Non integral olmasaydı diğerlerini sağlamadığı için sentaks hatası
}


CPPREFERENCE TAN CONCEPTLERE BAKTIK

BİR SONRAKİ DERS SPAN GÖRÜLECEK



*/
