/*

THREE-WAY COMPARASIONDA KALDIK

Bir operatör gibi görünsede getirdiği yenilik çok fazla.Karşılaştırma işlemlerine yönelik operator
funclarının programcı tarafından kolay yazılması için getirildi, tıpkı special member funclarda olduğu gibi.

C++20 ile karşılaştırma operatörlerininin durumu şu
6 tane karşılaştırma operatörü var

Ordering
<
<=
>
>=

equality operators
==
!=

----------------------------------------------------------------------------------------------------------------------------------------

C++20 YE GÖRE DURUM AŞAĞIDAKİ GİBİ

				EQUALİTY			ORDERİNG
PRIMARY			  	 ==				  <=>
SECONDARY		  	 !=				  <, >, <=, >=

Secondary lerin özelliği var.
1 - Secondaryler primary operator cinsinden derleyici tarafından yazılabiliyor

ÖR:
a != b    yi primary cinsinden yazıyor   !(a==b)


2 - Secondary orderinglerde threeway comparasion operatoru cinsinden yazabiliyor.
Yine < , > , >= , <= te threeway comparision kullanılarak yazılabiliyor.

3 - Birincil operatorlerde swap/reverse edilebiliyor 
a.operator(b) yerine b.operator(a) olabiliyor

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

1. FARKLILIK DERLEYİCİNİN == DEN HAREKETLE != İ KENDİSİNİN YAZMASI

class Mint
{
public:
	Mint(int x = 0) : mx(x) { }
	bool operator==(const Mint &other)const{
		return mx == other.mx;
	};

private:
	int mx;
};

int main()
{
	Mint m1{2}, m2{6};

	// C++ 20 öncesinde == vardı ama != yoktu. Yeni kurallar diyorki derleyici == kullanarak != i yazabilir.
	// Yani secondary olan primary cinsinden ifade edilebiliyor.

	auto b = m1 != m2; // C++20 DE GEÇERLİ.C++17 DE SENTAKS HATASI

}


2. PRIMARY OPERATORLERIN (== VE <=>) DEFAULT EDİLEBİLMESİ.YANİ DERLEYİCİ TARAFINDAN YAZILMASININ SAĞLANMASI

class Mint
{
private:
	Mint(int x = 0) : mx(x) { }
	bool operator==(const Mint &other)const = default

public:
	int mx;
};

int main()
{
	Mint m1{2}, m2{6};

	auto b1 = m1 == m2;
	auto b2 = m1 != m2; // GEÇERLİ. DERLEYİCİ == İ YAZIYOR VE != DE SECONDARY OLDUĞUNDAN PRIMARY CİNSİNDEN YAZILABİLİYOR.
			    // DERLEYICI SINIFIN VERI ELEMANLARINI BILDIRIM SIRASIYLA == ILE KARŞILAŞTIRIYOR
			    // VE BUNLARI LOGIC VE && ILE BAĞLIYOR.
}

DERLEYICI AŞAĞIDAKINI YAZIYOR

struct Nec{

	T tx;
	U ux;
	M mx;

	bool operator==(const Nec& other)const // DERLEYİCİ BU OPERATOR FUNCI DEFAULT ETTİ/YAZDI.
	{
		return tx == other.tx && ux == other.ux && mx == other.mx;	
	}

};

========================================================================================================================================
========================================================================================================================================

ORDERING TARAFINA BAKALIM

KARŞILAŞTIRMANIN KESIN SONUCU : Mesela C deki strcmp ve memcmp karşılaştırmanın kesin sonucunu veriyorlar.C++ ta stringe ait compare func var.
				Return değeri tamsayı. 1.büyük ise pozitif değer dönecek
				2.büyük ise negatif değer dönecek
				Eşit ise 0 dönecekti.
								
a				b
a > b
a < b
a == b


LEXICOGRAPHICAL COMPARE : 2 varlık karşılaştırılacak ama bunların elemanları var.Bunlar dizi olabilir yada struct olabilir ...
			  Hatta algorithm de lexicographical_compare funcı var.Bool döndürüyor ve 2 tane range alıyor.		  
			  1. range > 2. range ise boolean true döndürür.
			  Bu algoritma rangein karşılıklı öğelerini karşılaştırıyor.Eşit oldukça bir sonraki öğeye bakıyor
			  Eşit olması için eleman eleman eşit ve sayılarınında aynı olması gerekiyor.Erken biten küçük oluyor.

			  C++20 öncesinde containerların karşılaştırma operatorleride böyle çalışıyordu.

			  vector<int> ivec1(100'000u, 10);
			  vector<int> ivec2(1, 20);

			  std::cout << ivec1.size() << '\n';
			  std::cout << ivec2.size() << '\n';

			  std::cout << (ivec1 > ivec2); // True

			  <
			  >
			  >=
			  ...
			  Bunların hepsinin yerine three way comparision operatoru geldi.


SINIF IÇERISINDE LEXICOGRAPHICAL KARŞILAŞTIRMA YAZALIM.

struct Data
{
	int ix;
	unsigned ux;

	bool operator<(const Data& other) const
	{
		if(ix < other.ix)
			return true;
		if(other.ix < ix)
			return false;
		return ux < other.ux;
	}
};


----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

THREE-WAY COMPARASION OPERATOR C++20

Karşılaştırmanın kesin sonucunu veren binary bir operator

operator tokenı : <=>

int main()
{
	10 <=> 5  // 10 sol operand, 5 sağ operand
}

Bu operatorün ürettiği değer int türden değil.Bu karşılaştırma işleminin sonucu compare headerı içinde
tanımnlanan türlerden. Yani (10 <=> 5) bu bir sınıf türden ifade ve bu ifade 0 ile karşılaştırılabiliyor.
(10 <=> 5) > 0; yani burası legal

(10 <=> 5) > 0   ----> 1. büyük ise ifadeden elde edilen değer boolean true döner
(10 <=> 5) < 0   ----> 2. büyük ise ifadeden elde edilen değer boolean false döner
(10 <=> 5) <= 0
(10 <=> 5) >= 0
(10 <=> 5) == 0
(10 <=> 5) != 0  hepsi yazılabilir.

----------------------------------------------------------------------------------------------------------------------------------------

KARŞILAŞTIRILAN VARLIKLARIN TÜRÜNE BAĞLI OLARAK YENİ BAŞLIK DOSYAMIZ COMPARE DOSYASINDA OLAN TÜRLER

std::strong_ordering
std::weak_ordering
std::partial_ordering

NOT : Templatelerde partial ordering rules vardı.Daha spesifik olan templatein seçilmesi durumu. Onuna karıştırma
NOT2 : Sequence containerlarda strict weak ordering rules vardı. Equivalance denen karşılaştırmalar yapılıyordı. Onunlada bunu karıştırma


std::strong_ordering = 2 varlık ya biri büyük ya diğeri büyüK olmalı, ikiside değilse eşit olmalı.Burada strong equality var.

std::weak_ordering = Değerler fiilen eşit olmasada eşit kabul edilebiliyor.Böyle bir değere sahipse weak ordering kabul edilebilir.
		     ÖR: case insensitive string karşılaştırması
					 masa == MASA yı eşit kabul etme durumu.Burada bir equivalance durumu var.

std::partial_ordering = Sonuç büyükte küçükte eşitte olmak zorunda değil. Üçüde olabilir olmayadabilir.
			Reel sayı karşılaştırmasındaki not a number ile number olan değerin karşılaştırması yapılırsa
			sonuç 3 ihtimalden biri olmak zorunda değil.

----------------------------------------------------------------------------------------------------------------------------------------

SINIFI NASIL TASARLAMIŞLAR
--------------------------

int main()
{
	std::cout << typeid(10 <=> 5).name() << '\n'; // struct std::strong_ordering

	std::cout << typeid(1.0 <=> 2.3).name() << '\n'; // struct std::partial_ordering
}

SINIF IÇERISINDE CONSTEXPR STATIC VERI ELEMANLARI VAR
-----------------------------------------------------
- STRONG ORDERING için std::strong_ordering::equal, std::strong_ordering::less,std::strong_ordering::greater,std::strong_ordering::equivalent
  strong ordering için equal ve equivalent tamamen birbirine eşit.
 
- PARTIAL ORDERING için std::partial_ordering::less, std::partial_ordering::greater,std::partial_ordering::equavalent, std::partial_ordering::unordered

- WEAK ORDERING için std::weak_ordering::less, std::weak_ordering::greater, std::weak_ordering::equivalent var

----------------------------------------------------------------------------------------------------------------------------------------

ÖNEMLİ !!!!!!!!!!!!!!!!!!!!!!!!!!

(10 <=> 5) == std::strong_ordering::greater  Bu karşılaştırma true verecek.çünkü (10 <=> 5), std::strong_ordering::greater değerini üretiyor.
Tabi 0 sabiti ile hertürlü karşılaştırma işlemini de yapabiliyoruz.

Aşağıdakiler arasında bir fark yok
  
a > b			a <=> b > 0 
a < b			a <=> b < 0 
a >= b			a <=> b >= 0 
a <= b			a <=> b <= 0 

int main()
{
	int x = 12;
	int y = 20
		
	std::cout << boolalpha << (std::strong_ordering::greater > 0); //TRUE

	std::cout << boolalpha << (std::partial_ordering::unordered > 0); //FALSE
	std::cout << boolalpha << (std::partial_ordering::unordered < 0); //FALSE YİNE
}

Tüm karşılaştırmalar bu şekilde yapılabilir.

strong_ordering::less > 0  True olmak zorunda
strong_ordering::less == 0  false olmak zorunda
strong_ordering::less != 0  true olmak zorunda
strong_ordering::less >= 0  false olmak zorunda
... gibi

----------------------------------------------------------------------------------------------------------------------------------------

SPACESHIP OPERATÖRÜ PRIMARY COMPARE OPERATÖRÜ.
PRIMARY OPERATORLER REVERSE EDILEBILIYOR. SECONDARY OPERATÖRLERDE DERLEYICI TARAFINDAN PRIMARY CINSINDEN IFADE EDILEBILIYOR.

a == b        b == a		primary operator ve swap/reverse edebilir.
a != b		  !(a == b)     derleyici yazabilir bunuda.

a <=> b		  0 > (b <=> a)  Primary operator olduğundan yerdeğiştirebilir.

a < b		  (a <=> b) < 0  < secondary, primary cinsinden yazılabilir    b <=> a > 0 cinsinden yazılabilir.Yani reverse edilir.
BURADA ILK ÖNCE SECONDARY PRIMARY CINSINDEN YAZILDI, ELDE EDILEN PRIMARY DE REVERSE EDILEBILDIĞINDEN REVERSE EDILDI !!!!!

a <= b		  (a <=> b) <= 0			YADA   (b <=> a) >= 0 yazılabilir primary olduğundan yer değiştirebilir.


INTELLISENSE <=> OPERATORDE ALTINI ÇİZİYOR AMA HATA YOK

Öncelik parantezini kullanmak gerekiyor.

int x{}, y{345};
auto b = (x >> 5) & y;

----------------------------------------------------------------------------------------------------------------------------------------

DERLEYICI AŞAĞIDAKI KODLARI YAZACAK KENDISI 

struct A
{
	T t;
	U u;
	V v;

	bool operator==(const A& other)const
	{
		return t = other.t && u == other.u && v == other.v;
	}
	
	bool operator<(const A& other)const
	{
		if(t < other.t) return true;
		if(other.t < t) return false;
		if(u < other.u) return true;
		if(other.u < u) return false;
		return v < other.v; // Burada tekrar if e gerek yok. bir adımda true false elde edeceğiz.
	}
};


MOLA

========================================================================================================================================
========================================================================================================================================
========================================================================================================================================

Cppreferenceten bakıyoruz.

Strong_ordering : Bir birinin yerine geçebilirliği temsil ediyor, eğer a b ye equivalant ise (denk ise) f gibi bir func çağırdığımızda
a veya b ile bu function salt okuma amaçlı eriştiğinde bu nesnelere, bunların ikisi arasında hiçbir farklılık algılamayacak.
2 değer birbirine denk ise bunların arasında ayırt edici kriter yok.

Week_ordering : Birbirinin yerine geçebilirliği temsil etmiyor, eğer a b ye equivalant ise f(a) ile çağırmak ile f(b) çağırmakla
aynı anlamda olmak zorunda değil. 
Eşit kabul edilen değerler birbirinden ayrılabilen değerler. MASA ile masa eşit kabul edilebiliyor ama ayırt ediledebilir.

Partial_ordering : Birbirinin yerine geçebilirliği temsil etmiyor, weak ordering ile partial arasındaki farklılık aşağıda
karşılaştırılamayan değerleri kabul ediyor.
a < b, a ==b, a > b hepsi yanlış olabilir.

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

BIZ SADECE THREEWAY COMPARASION DEFAULT EDERSEK, DERLEYICI == IDE YAZAR.NORMALDE FARKLI KATEGORILER
BIRI EQUALITY DIĞERI ORDERING.BU DA PRATIKTE 6 OPERATORU BIRDEN KULLANABILECEĞIZ DEMEK

Bu operatorleri global, friend hatta constexpr durumunu çiğneyen durum yoksa otomatik olarak constexpr kabul ediliyor
hatta nodiscard attribute var gibi ele alınıyor.

class Wrapper{
private:
	int m_id;
public:
	constexpr Wrapper(int i)noexcept : m_id(i) { }
	auto operator <=>(const Wrapper &other)const = default;	// DİKKAT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
															// ARTIK 6 OPERATORUDE KULLANABİLİRİM
	
};

int main()
{
	Wrapper x{234}, y{526};
	
	std::cout << std::boolalpha;

	std::cout << "x == y : " << (x == y) << "\n";   // false
	std::cout << "x != y : " << (x != y) << "\n";   // true
	std::cout << "x > y : " << (x > y) << "\n";     // false
	std::cout << "x >= y : " << (x >= y) << "\n";   // false
	std::cout << "x < y : " << (x < y) << "\n";     // true
	std::cout << "x <= y : " << (x <= y) << "\n";   // true

	std::cout << typeid(x <=> y).name() << '\n';  // struct std::strong_ordering
}

Değerleri eşit yapıpta bakılabilir.




DERLEYİCİ NE YAPTI ? 
Derleyici aslında sınıfa 2 func yazdı.
1 - operator==   yazdı
2 - operator<=>   yazdı


DERLEYİCİNİN YAZDIĞI FUNCLAR AŞAĞIDA

class Wrapper{
private:
	int m_id;
public:
	constexpr Wrapper(int i)noexcept : m_id(i) { }
	constexpr bool operator==(const Wrapper &other)const noexcept
	{
		return m_id == other.m_id;
	}
	
	constexpr auto operator <=>(const Wrapper &other)const noexcept
	{
		return m_id <=> other.m_id; // bunlar int değerler olduğundan return değeri strong_ordering
	}								 // derleyicinin yazdığı func return değeride strong ordering oldu.
};


int main()
{
	Wrapper x{234}, y{526};
	
	std::cout << std::boolalpha;

	std::cout << typeid(x <=> y).name() << '\n';  // struct std::strong_ordering

	--------------------------------------------------------------------------------------------------

	Eleman int değilde double olsaydı bu durumda partial_ordering üretilecekti.
	Bu durumda func parametreside double olacaktı. constexpr Wrapper(double i)noexcept : m_id(i) { }
	olacaktı yani.Double non değeri demek, not a number değeri diğer değerlerden büyük yada küçük olmak
	zorunda değil.Bu sebeple partial oredering

	Wrapper x{4.5}, y{6.7};

	std::cout << typeid(x <=> y).name() << '\n';  // struct std::partial_ordering
}

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------

BİR NEDENDEN ÖTÜRÜ SPACESHIPİ KENDİMİZ YAZACAK OLURSAK

ÇOK ÖNEMLİ !!!!!!!!!!!!

class Person
{
public:
	auto operator<=>(const Person& other)const
	{	
		//IDIYOM
		if(auto cmp = m_first_name <=> other.m_first_name; cmp != 0) // 0 değilse ya strong_ordering::greater yada less olacak
			return cmp;
		
		if(auto cmp = m_last_name <=> other.m_last_name; cmp != 0) 
			return cmp;

		return m_id <=> other.m_id;
	}	

private:
	std::string m_first_name; // buradaki bildirim sırası bize uymazsa yada farklı şekilde karşılaştırmak istersek yazabiliriz.
	std::string m_last_name;
	int m_id;
};

DİKKAT : BURADA ARTIK == YAZILMIYOR. DERLEYICI YAZSAYDI == IDE YAZACAKTI !!!!!!!!!!!!!!!!!!!!!!!!!!

int main()
{
	Person p1,p2;

	auto b1 = p1 < p2;
	auto b2 = p1 <= p2;
	auto b3 = p1 > p2;
	auto b4 = p1 >= p2;
	auto b5 = p1 == p2; // SENTAKS HATASI BU. ÇÜNKÜ <=> İ BİZ YAZDIK VE == İ YAZMADIK. DERLEYİCİ YAZSAYDI == İDE YAZACAKTI.
}						// SENTAKS HATASIDA OLMAYACAKTI.


----------------------------------------------------------------------------------------------------------------------------------------

BURADA OPERATOR<=> YAZARKEN IÇERISINDE <=> KULLANDIK AMA BUNU <=> KULLANMADAN DA YAZABILIRIZ.
ZATEN <=> OPERATÖRÜ OLMAYAN NESNELER IÇIN KULLANAMAYIZ BUNU.

MESELA ÖRNEK VERELİM

class identity
{
public:
	bool operator<(const identity&) const;
	bool operator==(const identity&) const;
};

class Person
{
	identity m_id;
	int m_no;

	std::strong_ordering operator <=>(const Person & other)const
	{
		if(auto result = m_no <=> other.m_no; result != 0)
			result result;
		
		//return m_id <=> other.m_id; //SENTAKS HATASI ÇÜNKÜ IDENTITY İÇİNDE <=> OPERATOR YOK
		//KENDİMİZ YAZALIM AŞAĞIDA

		if(m_id == other.m_id)
			return std::strong_ordering::equal;
	
		if(m_id < other.m_id)
			return std::strong_ordering::less;

		return std::strong_ordering::greater;
	}
};

----------------------------------------------------------------------------------------------------------------------------------------

Biz mesela bir template kullanıyorsak yani identity bir template ise içerisinde threeway comparison var mı yokmu
(bunu if constexpr ile yaparız) sorgulaması yapacağız. Bunun için bir template daha yazmamız gerekecek. Bu standart değil.
C++23 te bu standart olacak
Bu bu hatayı biz giderelim

template<typename T,typename U>
auto synth_three_way(const T& lhs, const U& rhs)
{
	if constexpr(std::three_way_comparable_with<T,U>) // Bu trait compare header file içerisinde type traits değil
	{
		return lhs <=> rhs;
	}
	else
	{
		if(lhs == rhs)
			return std::strong_ordering::equal;
		if(lhs < rhs)
			return std::strong_ordering::less;
		
		return std::strong_ordering::greater;
	}
}

Bu durumda Personu editleyelim

class Person
{
	identity m_id;
	int m_no;

	std::strong_ordering operator <=>(const Person & other)const
	{
		if(auto result = m_no <=> other.m_no; result != 0) // resultı 0 ile eşitlemek zorundayız.
			return result;
		return synth_three_way(m_id, other.m_id);
	}
};

----------------------------------------------------------------------------------------------------------------------------------------

BIZ YUKARIDA RESULT != 0 YAZDIK BUNU YAPMANIN BIR YOLU DAHA VAR.

int main()
{
	int x = 10;
	int y = 20;

	x <=> y == 0; // yerine aşağıdakini yazabiliriz.

	is_eq(x <=> y); // Yukarıdaki ile bunun arasında fark yok.

	if(is_eq(x <=> y))
	
	if(is_lteq(x <=> y))
	
	if(is_gt(x <=> y))
	
	if(is_gteq(x <=> y))
		

}

----------------------------------------------------------------------------------------------------------------------------------------

SINIFLAR SÖZKONUSU OLDUĞUNDA GENELDE 3 KATEGORI VAR.

ordering			equality

İkiside var
sadece equality var - unordered set mesela
ikiside yok

----------------------------------------------------------------------------------------------------------------------------------------

DAHA STRONG BİR KARŞILAŞTIRMADAN DAHA WEAK E BİR DÖNÜŞÜM VAR

int main()
{
	weak_ordering result1 = 10 <=> 20; // GEÇERLİ. Demekki strongtan weak e dönüşüm var
	partial_ordering result2 = 10 <=> 20; // GEÇERLİ. Demekki strongtan partiale dönüşüm var

	// Weak orderingte partial orderinge dönüşebiliyor.Ama tersi mümkün değil.
	weak_ordering result3 = 1.2 <=> 3.4; // partial orderingten weake dönüşüm yok.
}

----------------------------------------------------------------------------------------------------------------------------------------

class Person
{
	std::string gm_name;
	double mval;

	auto operator<=>(const Person& rhs)const
	{
		if(auto cmp = m_name <=> rhs.m_name; cmp)
			return cmp;
		
		return mval<=>rhs.mval;	 // Sentaks hatası çünkü auto return type var.Yukarıdaki strong ordering
	}							 // aşağıdaki ise partial ordering türünden
};								 

Yukarıdaki hatayıda çözmenin yolu var. 
1 - return değeri partial_ordering yapılır.
2 - trailing return type kullanılabilir.

1.si kolay partial_ordering operator<=>(const Person& rhs)const
			...

2.si 
class Person
{
	std::string gm_name;
	double mval;

	auto operator<=>(const Person& rhs)const -> std::common_comparison_category_t<decltype(m_name <=> rhs.m_name), decltype(mval <=> rhs.mval)>
	{
		if(auto cmp = m_name <=> rhs.m_name; cmp)
			return cmp;

		return mval<=>rhs.mval;	 // GEÇERLİ
	}							 
};

ikiside strong ise strong
biri strong diğeri weak ise weak ordering olacak
biri partial diğeri başka birşeyse partial ordering olacak

MOLA

========================================================================================================================
========================================================================================================================
========================================================================================================================
========================================================================================================================

ÖR: Partial Ordering

struct Myint{
	std::optional<int> mval = nullopt;

	bool operator==(const Myint& other)const
	{
		if(!mval || !other.mval) // değer tutmuyorsa optional nesnelerinden biri veya ikisi false dönecek
			return false;

		return *mval == *other.mval;
	}

	std::partial_ordering operator<=>(const Myint& other)const
	{
		if(!mval || !other.mval)
			return std::partial_ordering::unordered;

		return *mval <=> other.mval;
	}
};

int main()
{
	auto result = Myint{12} <=> Myint{4}; // True olacak yani partial_ordering::greater

	-------------------------------------------------------------------------------------

	std::cout << boolalpha <<(Myint{123} < Myint{}) << '\n'; // False
	std::cout << boolalpha <<(Myint{123} > Myint{}) << '\n'; // False
	std::cout << boolalpha <<(Myint{123} == Myint{}) << '\n'; // False

}

----------------------------------------------------------------------------------------------------------------------------------------

BURADA FUNCTIONUN CONSTEXPR OLMASI DURUMU

class Mint{
public:
	explicit constexpr Mint(int val) : mval{val} { }
	
	auto operator<=>(const Mint&)const = default;

private:
	int mval;
};

template<typename T>
constexpr bool is_less_than(const T&lhs, const T&rhs)
{
	return lhs < rhs;
}

int main()
{
	std::cout << std::boolalpha;

	constexpr Mint mint1(208);
	constexpr Mint mint2(751);

	constexpr auto b = is_less_than(mint1, mint2);  //Geçerli.
}

NOT: Constexpr koşulunu sağlıyorsa zaten doğrudan constexpr oluyor. Daha önce yazmıştık

----------------------------------------------------------------------------------------------------------------------------------------

BİR SINIF İÇİN SPACESHIP OPERATÖRÜNÜ OLUŞTURUP SONRADA O SINIF TÜRDEN NESNELERİ VECTORDE TUTALIM
SONRADA VECTORU SORT ALGORİTMASIYLA SORT EDELİM.

#include<vector>
#include<compare>
#include<algorithm>
#include "nutility.h"

struct Point{
	int x{};
	int y{};
	int z{};
	auto operator<=>(const Point &p)const = default;
};

int main()
{
	using namespace std;

	vector<Point> pvec;
	Irand irand{ 0,9 };

	for(int i = 0; i < 20; ++i)
	{
		pvec.emplace_back(irand(), irand(), irand());
	}

	sort(pvec.begin(), pvec.end());

	for(const auto &[x,y,z] : pvec)
	{
		std::cout << x << " " << y << " " << z << "\n"; 
	}
}

ÇIKTI
-----
1 4 1
2 1 2
2 1 3
2 4 1
2 4 9
3 6 4
3 6 8
4 0 7
4 1 8
4 9 9
5 1 5
5 6 6
6 0 6
6 5 6
7 2 7
7 3 7
8 1 6
9 1 3
9 4 2
9 4 4


*/
