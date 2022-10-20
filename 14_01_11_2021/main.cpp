/*

Compile timeda değer hesaplamaya yönetlik birtek constexpr functlar yok. Bunlardan başka yolda var.
Eskiden constexpr func yoktu ama bu yine yapılıyordu.

HEsaplayacağımız değer bir metafunction olarak kullanılacak sınıfın value isimli elemanında tutulacak.
Bu tipik constexpr static veri elemanı olacak.

Bize bir tamsayının bir tasayı kuvvetini compile timeda hesaplama için kullanılacak bir sınıf olsun. Sınıf dedik ama böyle sınıflara metafunction diyoruz.
Bunun varlık nedeni compile timeda değer hesaplamak. Metafunctionlarda base case ya explicit specialization yada partial specialization yapılıyor.

template<unsigned int base, unsigned int exp>
struct Power{
	constexpr static unsigned value = base * Power<base,exp-1>::value;  // Burası üstel işlemi yapan kısım
};

template<unsigned base>
struct Power<Base,0>
{
	constexpr static unsigned value = 1;
}

BURASI BIR METAFUNCTION. BU BIR DEĞER ÜRETIYOR. BUNU VALUE ELEMANINA ERIŞEREK KULLANIYORUM.
VARIABLE TEMPLATE ILE BUNU DOĞRUDAN VALUE ELEMANINA ERIŞMEK YERINE VALUE ELEMANINA KARŞILIK GELEN BIR VARIABLE ILE YAZABILIRIZ.

template <unsigned base, unsigned exp>
constexpr unsigned Power_v = Power<base, exp>::value;

int main()
{
	// auto constexpr val = Power<3,7>::value; Variable template aşağıda kullanılarak daha pratik yazılabilir.
	auto constexpr val = Power_v<3,7>;
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

METAFUNCTIONLARLA İLGİLİ HEADER FİLE TYPE TRAITS

Bu modern C++ ın en önemli modüllerinden biri. type traitste metafunclar var. Bunların bir kısmı boolean değer üretmeye yönelik, bir kısmıda tür üretmeye yönelik.

template<typename T>
struct xyz{
	constexpr static bool value = true;
};


ÖR:
int main()
{
	using namespace std;

	is_pointer_v<int>::value; // false
	is_pointer_v<int*>::value; // true
}

Metafunction nasıl yazılır ve bunlar nasıl kullanılır farklı kavramlar.Metafunctionı kullanmanın birsürü yolu var.
Mesela STATIC ASSERT. Compile time ifadesi veriyoruz bu ifade compile timeda logic olarak yorumlanıyor yanlışsa sentaks hatası oluşuyor.

template<typename T>
void func(T x)
{
	static_assert(sizeof(T) == 4,"type must be 4 bytes");	//C++17 öncesinde buraya birde string literali vermek gerekiyordu.Sonrasında gerek yok

	-----------------------------------------------------------------------------------------------------------------------------------------------

	static_assert(std::is_pointer<T>::value); // true olursa içerideki static assert fail olmaz, false olursa static assert fail olacak.
}

int main()
{
	func(4.5); //Sentaks hatası

	-----------------------------------------------------------------------------------------------------------------------------------------------

	func((int*)nullptr);
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEMPLATEIN YANİ METAFUNCIN KENDİSİ NASIL YAZILIYOR

İntegral constanta bakalım yine.T bir tür v de o türden sabit.

template<class T, T v>
struct integral_constant {
	static constexpr T value = v;
	using value_type = T;
	using type = integral_constant; // using injected-class-name
	constexpr operator value_type() const noexcept { return value; }
	constexpr value_type operator()() const noexcept { return value; } // C++14
};

template <bool B>
using bool_constant = integral_constant<bool, B>;  //C++ 17

bunlar doğrudan tür eş ismi.Template değil :D:D Karışmasın
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

Aşağıda örnekleri var.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


int main()
{
	integral_constant<int,5>; // T int, v 5 
	integral_constant<int,5>::value; // bu değer 5 ve biz bunu compilşe timeda herhangibir yerde kullanabilirim.Mesela array boyutu olabilir.

	---------------------------------------------------------------------------------------------------------------------------------------------

	integral_constant<bool,true>; // T bool, v true
	integral_constant<bool,true>::value; // True
	integral_constant<bool,false>::value; // False

	---------------------------------------------------------------------------------------------------------------------------------------------

	C++20 ile nontype parameter reel sayı türünden de olabilir.
	
	integral_constant<double,5.7>; // T double olacak, v 5.7 olacak

	---------------------------------------------------------------------------------------------------------------------------------------------

	C++17 ile eklenen alias template var.

	integral_constant<bool,true>; //yazmak yerine 

	bool_constant<true>; // yazılabilir
	bool_constant<false>; // yazılabilir.

	---------------------------------------------------------------------------------------------------------------------------------------------

	Bizi ilgilendiren daha çok true_type ve false_type

	using true_type = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;

	---------------------------------------------------------------------------------------------------------------------------------------------

	integral_constant<bool, true>::value_type; // value_type bool
	
	integral_constant<bool, true>::type; // Sınıfın typeı integral_constant<bool,true> demek.Bir sınıf şablonu içerisinde o sınıf adı geçerse
										 // doğrudan sınıf açılımı ifadesi gibi işlem görüryor. TEMEL C++

	integral_constant<bool, true>::value; // True

	---------------------------------------------------------------------------------------------------------------------------------------------

	TÜR DÖNÜŞTÜRME OPERATOR FUNC.ÇOK KRİTİK !!!!

	constexpr operator value_type() const noexcept { return value; }
	
	Tür dönüştürme operator functa value_type birinci template parametresi olduğuna göre, template integral_constant<bool,true> olsun, bu durumda 
	value_type bool olacak. function aşağıdaki hale dönüşecek.
	
	constexpr operator bool() const noexcept { return value; };  

	<int,5> açılımı olsaydı aşağıdaki olacaktı
	constexpr operator int() const noexcept { return value; };  

	Bu funclar constexpr olduğundan bunlar explicitly veya implicitly compile time sabiti ve funcların return değeri yine value.

	---------------------------------------------------------------------------------------------------------------------------------------------

	integral_constant<int,5>{} +  integral_constant<int,9>{}; // Bu durumda toplama operatörünün operandları olduğundan operator value type 
								  // kullanılarak int türüne dönüştürülecek ve bunlar constexpr funclar olduğundan
								  // buradaki ifadeler compile time sabiti olacak.
	
	---------------------------------------------------------------------------------------------------------------------------------------------

	FUNC CALL OPERATOR FUNC

	constexpr value_type operator()() const noexcept { return value; } // integral constant nesnesi func call operatörünün operandı olunca
									   // bu function constexpr, geri dönüş değeri value type,
									   // sınıfın const member functionu ve noexcept
}

VALUE'YU ELDE ETMENIN 3 YOLU VAR
- Sınıfın value elemanını ismiyle kullanmak
- İntegral constant türünden nesnenin tür dönüştürme operator functionunu çağırmak (operator value_type()) 
- Func call operator functa doğrudan value veriyor.


--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ŞİMDİ ORİGİNALİNE BAKALIM

int main()
{
	constexpr integral_constant<int,5> x;
	constexpr integral_constant<int,y> y;

	constexpr auto val = x + y; // val 12 oldu.
	
	---------------------------------------------------------------------------------------------------------------------------------------------

	constexpr auto val = x() + y(); // SENTAKS HATASI YOK.Burada da func call operatorü. 12 çıkar.

	---------------------------------------------------------------------------------------------------------------------------------------------

	//true_type
	//integral_constant<bool,true>

	//false_type
	//integral_constant<bool,false>
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
struct isFloatingPoint : true_type
{
}

int main()
{
	isFloatingPoint<int>::value; // bu aslında true type in valuesu truedur. False typetan üretilirse valuesu false tur
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DİKKAT !! GÜZEL ÖRNEK TEMEL C++TA DA VARDI

template <typename T>
struct isFloatingPoint : false_type
{
}

template<>
struct isFloatingPoint<double> : true type
{
	
}

template<>
struct isFloatingPoint<float> : true type
{
	
}

template<>
struct isFloatingPoint<long double> : true type
{
	
}

int main()
{
	isFloatingPoint<int>::value; // false
	isFloatingPoint<double>::value; // True
	isFloatingPoint<float>::value; // True
}

Compile timeda template argümanı türü floating point olurmu sorusunun cevabı.
Burada eksiklik var. volatile const volatile const larda var.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
constexpr bool IsArray_v = std::is_array<T>::value;

int main()
{
	is_array_v<int[10]>;
	is_array_v<double[10]>;
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DAHA KAPSAMLI HALİ AŞAĞIDA

#include <type_traits>
#include <iostream>

template<typename T>
struct IsArray : std::false_type {};

template<typename T>
struct IsArray<T[]> : std::true_type {}; // incomplete typelar için partial spec

template<class T, std::size_t N>
struct IsArray<T[N]> : std::true_type {}; // complete typelar için partial spec

template<typename T>
constexpr bool IsArray_v = IsArray<T>::value; //variable template

int main()
{
	std::cout << std::boolalpha;	
	
	std::cout << IsArray_v<int> << "\n";	//false
	std::cout << IsArray_v<int*> << "\n";	//false
	
	std::cout << IsArray_v<int []> << "\n"; //true
	std::cout << IsArray_v<const int []> << "\n"; //true
	std::cout << IsArray_v<int [5]> << "\n"; //true
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

POINTERLAR İÇİN

#include <type_traits>

template<typename T>
struct is_pointer_helper : std::false_type {};

template<typename T>
struct is_pointer_helper<T*> : std::true_type {};

template<typename T>
struct is_pointer : is_pointer_helper< typename std::remove_cv<T>::type > {}; // Olay burada dönüyor. ispointer helper burada remove_cv ile constluk ve volatile olma özelliğini 
									      // kaldırıyor ve pointer mı diye bakıyor. Burada constta volatile da olsa pointer var ise pointer kabul edilir.


int main()
{
	using namespace std;
	std::cout << std::boolalpha;

	constexpr auto x =  is_pointer<volatile int*>::value; // True
	constexpr auto y =  is_pointer<const volatile int*>::value; // True
	constexpr auto z =  is_pointer<const int*>::value; // True
	
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

IS CONST

#include <type_traits>

template<typename T> struct is_const : std::false_type {};
template<typename T> struct is_const<const T> : std::true_type {};

En çok kullanılan teknik bu. Defaultta false type tan kalıtımla elde etmek. True değer sözkonusu olanları true type tan elde edilir.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BURADAKİ TEKNİĞE DİKKAT

#include <type_traits>
#include <iostream>

template< class T >
struct IsFloatingPoint : std::integral_constant<bool,
	std::is_same<float, typename std::remove_cv<T>::type>::value ||
	std::is_same<double, typename std::remove_cv<T>::type>::value ||
	std::is_same<long double, typename std::remove_cv<T>::type>::value> { };  // T türünün const ve volatile olma durumunu varsa silmiş sonra bu tür float, double veya long double türlerinden
																			  // birine eşitse (bunuda is_same ile kıyaslamış), buradan true dönecek ve integral_constant<bool,true> açılımdan
																			  // kalıtımla elde edilmiş bir template olacak bu template. Değilse false olacak. Bizde bunu sorguluyoruz.
template< class T >
inline constexpr bool isFloatingPoint_v = IsFloatingPoint<T>::value; // Burada da _v li yazmışız.

int main()
{
	std::cout << std::boolalpha;

	std::cout << isFloatingPoint_v<int> << "\n"; //false
	std::cout << isFloatingPoint_v<float> << "\n"; //true
	std::cout << isFloatingPoint_v<const float> << "\n"; //true
	std::cout << isFloatingPoint_v<double> << "\n"; //true
	std::cout << isFloatingPoint_v<double const> << "\n"; //true
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

IS SAME

#include <type_traits>
#include <iostream>
#include <cstdint>

//is_same tür ilişkisi trait'inin gerçekleştirimi

template<class T, class U>
struct IsSame : std::false_type {};

template<class T>
struct IsSame<T, T> : std::true_type {};

template<typename T, typename U>
inline constexpr bool IsSame_v = IsSame<T, U>::value;

int main()
{
	std::cout << std::boolalpha;
	std::cout << IsSame_v<int, int> << "\n"; //true
	std::cout << IsSame_v<int, const int> << "\n"; //false
	std::cout << IsSame_v<int, unsigned> << "\n"; //false
	std::cout << IsSame_v<int, std::int32_t> << "\n"; //true or false
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
struct IsVoid : std::is_same<void, std::remove_cv_t<T>> {};

template<typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

int main()
{
	std::cout << std::boolalpha;
	std::cout << IsVoid_v<int> << "\n";
	std::cout << IsVoid_v<void> << "\n";
	std::cout << IsVoid_v<const void> << "\n";
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

İS NULL POINTER

template<typename T>
struct is_null_pointer : std::is_same<std::nullptr_t, std::remove_cv_t<T>> {};

======================================================================================================================================================================================
======================================================================================================================================================================================
======================================================================================================================================================================================
======================================================================================================================================================================================
======================================================================================================================================================================================


2. GRUP TEMPLATELER  -  TYPE TRANSFORMATION

Burada bizden bir type alıp bir type verecek. Öncekinde type alıp değer döönüyordu. const mu değil mi ? true yada false gibi...
Tamsayı türü mü ? evet yada hayır

add_const remove_const 
Burada is_aggregate diye bir metafuncta var.Yani bir sınıf

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Eğer bu sınıf türünden nesneler memcopy gibi bir funcla copy edilebilir nitelikteyse eleman eleman koplayalama yerine
bellek bloğunu memcopy ile kopyalayayım diyor. Ama bu karar compile timeda veriliyor.

is_standart_layout, bu bize bazı garantiler veriyor. Derleyici bazı kod seçimlerinde diyecekki bu kodu standart layouta giriyor. Bu işlemler UB oluşturmayacak.
Tıpkı move ctorun noexcept olmasına bakıp farklı kod üretmesi gibi.Optimizasyon 2 taneydi Temelde 100 kere gördük.Birincisi oluşturulan assembly kod optimizasyonu,
ikincisi ise daha farklı çalıştırılacak kodun compile timeda seçilmesi.

2. Ye örnek verelim.

template <typename Iter>
void func(Iter beg, Iter end)
{
	int n = 5;

	beg += n; // İterator random access ise bu legal ama Bidirectional ise geçerli olmayacak.Böyle bir func şablonu var. Advance ismi

	advance(beg,n); // Arka plandaki kod compile timeda saptanıyor.Eğer beg random access ise buradaki advance, beg+= n; koduna dönüşüyor ama iter bidirectional ise
					// bu kod döngü içerisinde beg i n kere artırıyor. 
					// Yani buradaki iterator random access mi değil mi buna bakmamız gerekir.
}

Iterator kontrolü nasıl yapılır ?
1- Tag dispatch
2- if constexpr 
3- Sfinae

Sonuçta bu random access iterator mu değil mi onu anlamamız lazım. Hepsi Compile time araçları tabi bunlar.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	vector<int> ivec(100);

	generate(ivec.begin(), ivec.end(),[]{return rand();});

	for(auto n : ivec)
	{
		std::cout << val << " ";
	}

	copy(ivec.begin(), ivec.end(), ostream_iterator<int>{cout," "}); // böylede yazdırdık.

	-------------------------------------------------------------------------------------------------------------------------------------
	
	vector<unsigned int> ivec(100); // dikkat.int olunca negatif değerler geldi ondan unsigned yaptık.

	mt19937 eng;

	generate_n(back_inserter(ivec), 20,[]{return eng();}); // Burada eleştirilecek şey ? eng nin sizeı 5000 byte. Burada 5000 bytelık kopyalama var. 
								// bunu std:ref(eng) ile göndermek mantıklı !!!!

	copy(ivec.begin(), ivec.end(), ostream_iterator<int>{cout," "}); 

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÇOK BASİTCE ADVANCE İ YAZALIM

template <typename Iter>
void Advance(Iter& iter, int n)
{
	while(n--)
	{
		++iter;
	}
}
- yi yazmadık bile


--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BURADA 3 TEKNIĞEDE BAKALIM.
1. TAG DISPATCH TEKNIĞI.
   FUNCTİON OVERLOADING İLE YAPILACAK

NOT !!!!!!!!!!!!! TEMELDE DETAYLI GÖRÜLDÜ
typename Iter::iterator_category; Iterator categorysini veriyordu.Mesela random access iterator ise iterator categorisi random_access_iterator_tag
bidirectional iterator ise bidirectional_iterator_tag

NOT!!!!!!!!!!!!!!!!!!!!!!!!!!
iterator_traits kullanınca ayrıca tag'leride gösteriyor.
Tagleri görmek için ayrıca Iter::iterator_category isimli type ına erişerekte tag bilgisini alırız.
iterator traits pointerlar içinde tagleme yapabiliyor.Aşağıda yazıldı.


NOT !!!!!!!!!!!!!
typename Iter::iterator_category
typename std::iterator_traits<Iter>::iterator_category // Neden Bu var ???

NEDEN YUKARIDAKI GIBI DEĞILDE AŞAĞIDAKI GIBI YAZILDI.ITERATOR_TRAITS POINTER TÜRLERI IÇIN KULLANILIYOR.
EĞER ITERATOR POINTER TÜRÜ ISE ITERATOR_CATEGORY NESTED TYPE I OLMAYACAK AMA ITERATOR_TRAITS TEMPLATE ILE
BUNUN BIR ITERATOR KATEGORISI OLMASINI SAĞLAMIŞLAR. BUNU PARTIAL SPECIALIZATION ILE BUNU YAZMIŞLAR.
POINTER TÜRÜ BURADA RANDOM ACCESS ITERATOR KATEGORISINDE !!!!!!!!!!!!!!!!!!!!


template <typename Iter>
struct IteratorTraits // Burda doğrudan iterator category yi alacak
{
	using iterator_category = typename Iter::iterator_category;
};													

template<typename Iter>
struct IteratorTraits<T*> // Pointer türleri için bu PARTIAL SPECIALIZATION var.
{
	using iterator_category = std::random_access_iterator_category;	
}

int main()
{
	iterator_traits<vector<int>::iterator>::iterator_category;
	iterator_traits<int*>::iterator_category;
}

Yukarıdaki iterator traitslerin gerçek olanları aşağıda kullanılacak.Fikir vermek amaçlı yazıldı.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


1. YÖNTEM TAG DISPATCHTEN DEVAM

using namespace std;

template<typename Iter>
void Advance_impl(Iter& iter, int n, std::random_access_iterator_tag) // son parametre kullanılmayacak o yüzden isim verilmiyor.
{
	std::cout << "for random access iterator\n";
	iter += n;
}

template<typename Iter>
void Advance_impl(Iter& iter, int n, std::bidirectional_iterator_tag) // son parametre kullanılmayacak o yüzden isim verilmiyor.
{
	std::cout << "for random access iterator\n";

	while(n--)	// bunun koduda böyle olsun biraz farklı
		++iter;
}

template <typename Iter>
void Advance(Iter& iter, int n)
{
	//typename std::iterator_traits<Iter>::iterator_category;

	Advance_impl(iter, n, typename std::iterator_traits<Iter>::iterator_category{}); // Bu ifadeyi gönderme sebebim func overloadingte seçim yapmak.
}

int main()
{
	vector<int> ivec(199);

	auto iter = ivec.begin();

	Advance(iter,10); 

	list<int> ilist(100);
	auto list_iter = ilist.begin();

	Advance(list_iter,15);

}

BURADA COMPILE TIMEDA KOD SEÇTİK.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

2. YÖNTEM IF CONSTEXPR

template <typename Iter>
void Advance(Iter& iter, int n)
{
	if constexpr(std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>)
	{
		std::cout << "for random access iterator\n";
	}
	else // ELSE IF CONSTEXPR YERİNE ELSE IF YAZARSAK, IF CONSTEXPR OLMAYANA DÖNÜŞÜR. :D:D:D BU ÖRNEKTE ELSE YAPTIK
	{
		std::cout << "other iterator categories\n";
	}
}

int main()
{
	vector<int> ivec(199);

	auto iter = ivec.begin();

	Advance(iter,10);

	list<int> ilist(100);
	auto list_iter = ilist.begin();

	Advance(list_iter,15);
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

3. SFINAE - EN KARMAŞIĞI BU

Random accesste substitutionda hata olmayacak, random access iterator değilse substitutionda hata olacak.
Bunun için enable_if kullanacağız.Enable_if i 3 ayrı yerde kullanabiliyoruz.

ENABLE IF NEYDİ?
true açılımı için partial spec i var ama 
enable if in false için burada type yok. void yapılmış defaultta.

Yani template substitution kısmında enable if kullanırsak ve type ın olmamasından dolayı bir hata oluşursa derleyici bir hata vermeyecek bizim overloadu 
overload setten çıkartacak

ÖR:
template<typename Iter>
enable_if_t<is_same_v<std::random_access_iterator_tag,typename Iter::iterator_category>>
Advance(Iter &iter, int n)
{
	std::cout << "for random access iterator\n";
}

template<typename Iter>
enable_if_t<is_same_v<std:bidirectional_iterator_tag,typename Iter::iterator_category>>
Advance(Iter &iter, int n)
{
	std::cout << "Bidirectional access iterator\n";
}

int main()
{
	list<int> ilist(100);

	auto list_iter = ilist.begin();

	Advance(list_iter, 10); // 2. overload çağrılır.

	----------------------------------------------------------------------------------------------------------------------------------------

	vector<int> ivec(100);

	auto vec_iter = ivec.begin();

	Advance(vec_iter,10); // 1. overload çağrılır.

}

SFINAE DAHA SONRA 1 SAAT KADAR IŞLENECEK

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SFINAE UYGULAMANIN TEK YOLU BU DEĞİL. NERELERDE UYGULANABİLİR ?
1. RETURN DEĞERİ OLARAK KULLANDIK

template <typename Iter>  //alias template
using IfRandomAccessIterator = enable_if_t<is_same_v<std::random_access_iterator_tag,typename Iter::iterator_category>>; // tür eş simi verdik kısa olsun diye

template<typename Iter>
IfRandomAccessIterator<Iter> Advance(Iter &iter, int n)  // böylede yazılabilir.
{
	std::cout << "for random access iterator\n";
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

2. DEFAULT ARGUMAN OLARAK VERILDI

KOdda hata olabilir genel tekniği göstermeye çalışıyor hoca.

template <typename Iter>  //alias template
using IfRandomAccessIterator = enable_if_t<is_same_v<std::random_access_iterator_tag,typename Iter::iterator_category>>

template <typename Iter, typename = IfRandomAccessIterator> // SUBSTITUTION YAPILIRKEN HATA OLURSA YİNE SFINAE OUT OLACAK
Advance(Iter& iter, int n) //BUrada da kullanılabilir.
{
	//...
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

3. OLARAKTA FUNC PARAMETRESİNE DEFAULT POINTER PARAMETRE

template <typename Iter)
Advance(Iter &iter, int n , enable_if_t<is_same_v<std::random_access_iterator_tag,typename Iter::iterator_category>> *p = nullptr)
{

	//..

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DAHA KISA 3 ÖRNEK

1.
template <typename T>
enable_if_t<is_integral_v<T>> fucn(T x) // substitution aşamasında is_integral false verirse type ı olmadığından(void) overlaod setten çıkartılacak
{
	
}

2.
template <typename T, typename = enable_if_t<is_integral_v<T>>> // Burada 2. template param default arguman aldı, buda substitutionda is integral v açılımı false ise SFINAE OUT olacak
T func(T x) 
{
	
}

3.
template<typename >
T func(T x, enable_if_t<is_integral_v<T>> *p = nullptr)
{
	
}


BUNLARIN UZUN ÖRNEKLERİ GİTHUBDA
MOLA

====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================

Bu func şablonunu çift sayı ve tek sayı n değerleri için farklı kodlar oluşturabilir miyiz?

YÖNTEM
template <int n>
void func()
{
	if constexpr(n % 2 == 0)
	{
		std::cout << "cift\n";	
	}
	else{
		std::cout << "Tek\n";	
	}
}

DIĞER YÖNTEM
C deki multidimensional array pointer ile nasıl tutulur olayı.Pointer semantiği. C++taki referans semantiği karşılığı
Örneğin özü [0] boyutlu dizi olamazdan geliyor :D

template <int n>
void func(int(*p)[n % 2 == 0] == nullptr)  // tekler için bu sfinae out oldu
{
	std::cout << "for even numbers " << n << "\n";
}

template <int n>
void func(int(*p)[n % 2 != 0] = nullptr) // çiftler için bu sfinae out oldu
{
	std::cout << "for odd numbers " << n << "\n"
}

int main()
{
	func<12>();

	func<7>(); // Bununla yukarıdaki farklı kodları çalıştıracak
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TYPE ALIP TYPE VERENLERE DEVAM

Bu type verenlerin return değerini iletmesi value elemanı ile değil type alias ile oluyor.

int main()
{
	remove_reference_t<int&>::type // int türü.
	remove_reference_t<int&&>::type // int türü.
}


YAZALIM REMOVE REFERENCI

template <typename T>
struct RemoveReference {
	using type = T;
};

template <typename T>
struct RemoveReference<T&>
{
	using type = T;
};

template <typename T>
struct RemoveReference<T&&>
{
	using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;



int main()
{
	RemoveReference_t<int>::type // int türü.
	RemoveReference_t<int&>::type // int türü.
	RemoveReference_t<int&&>::type // int türü.
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

REMOVE CONST

template <typename T>
struct remove_const {
	using type = T;
};

template <typename T>
struct remove_const<const T> {
	using type = T;
};

template <typename T>
using remove_const_t = typename remove_reference<T>::type;

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

REMOVE POINTER

template <typename T> struct remove_pointer { typedef T type; };
template <typename T> struct remove_pointer<T*> { typedef T type; };
template <typename T> struct remove_pointer<T* const> { typedef T type; };
template <typename T> struct remove_pointer<T* volatile> { typedef T type; };
template <typename T> struct remove_pointer<T* const volatile> { typedef T type; };

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::typ

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ADD CV

template <typename T> struct add_cv { using type = const volatile T; };
template <typename T> struct add_const { using type = const T ; };
template <typename T> struct add_volatile{ using type = volatile T; };

template<typename T>
using add_cv_t = typename add_cv<T>::type;

template<typename T>
using add_const_t = typename add_const<T>::type;

template<typename T>
using add_volatile_t = typename add_volatile<T>::type;


int main()
{
	add_cv_t<int> x1{};
	add_cv_t<int *> x2{};
	add_const_t<int *> x3{}; // türü int * const
	add_const_t<const int *> x4{}; // türü const int * const
	add_volatile_t<int *> x5{};
	add_volatile_t<const int *> x6{};
}

CPPREFERENCE TA TYPE TRAITS KISMININ ÜSTÜNDEN GEÇTİ HOCA

*/
